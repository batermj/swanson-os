/* Copyright (C) 2017 - 2018 Taylor Holberton
 *
 * This file is part of Swanson.
 *
 * Swanson is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Swanson is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Swanson.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <swanson/kernel.hpp>

#include <swanson/bad-instruction.hpp>
#include <swanson/exception.hpp>
#include <swanson/elf.hpp>
#include <swanson/process.hpp>
#include <swanson/stream.hpp>

#include "fs/ramfs/file.h"
#include "debug.h"
#include "disk.h"
#include "gpt.h"
#include "rstream.h"

#include <cstdlib>
#include <cstring>

namespace {

class InitStream final : public swanson::Stream{
	const void *data;
	uint64_t size;
	uint64_t offset;
public:
	InitStream(const void *data_, uint64_t size_) noexcept {
		data = data_;
		size = size_;
		offset = 0;
	}
	~InitStream() {

	}
	void Read(void *buf, uint64_t bufSize) {

		if ((bufSize + offset) >= size)
			throw swanson::Exception("Overflow has been detected.");

		auto data8 = (const unsigned char *) data;

		std::memcpy(buf, &data8[offset], bufSize);

		offset += bufSize;
	}
	void SetPosition(uint64_t position) {
		offset = position;
	}
	void Write(const void *, uint64_t) {
		throw swanson::Exception("Write operations not permitted.");
	}
};

} // namespace

namespace swanson {

Kernel::Kernel() noexcept {
	ramfs_init(&initramfs);
}

Kernel::~Kernel() {
	ramfs_free(&initramfs);
}

void Kernel::AddDisk(std::shared_ptr<Disk> disk) {
	disks.emplace_back(disk);
}

void Kernel::LoadInitRamfs(const void *buf, uintmax_t buf_size) {

	struct rstream rstream;
	struct stream *stream;

	/** The 'rstream' represents a stream that
	 * contains the initramfs serialized data.
	 * */

	rstream_init(&rstream);

	rstream_setbuf(&rstream, buf, buf_size);

	stream = rstream_to_stream(&rstream);

	ramfs_decode(&initramfs, stream);
}

ExitCode Kernel::Main() {

	std::shared_ptr<Stream> initStream;

	if (root_fs == nullptr) {
		throw Exception("Root file system not mounted.");
	}

	auto exitCode = root_fs->OpenFile("/bin/init", initStream);
	if (exitCode != ExitCode::Success)
		return exitCode;

	elf::File elfFile;

	elfFile.Decode(*initStream);

	auto process = std::make_shared<Process>();

	process->Load(elfFile);

	AddProcess(process);

	// Run until the first process
	// exits.
	while (!process->Exited()) {
		Step(100);
	}

	return ExitCode::Success;
}

void Kernel::SetRootFS(std::shared_ptr<vfs::FS> root_fs_) {
	root_fs = root_fs_;
}

void Kernel::Step(uint32_t steps) {
	auto processID = 0UL;
	for (auto &process : processes) {
		try {
			process->Step(steps);
		} catch (BadInstruction &badInstruction) {
			badInstruction.SetProcessID(processID);
			throw;
		}
		processID++;
	}
}

void Kernel::AddProcess(std::shared_ptr<Process> &process) {
	processes.emplace_back(process);
}

} // namespace swanson
