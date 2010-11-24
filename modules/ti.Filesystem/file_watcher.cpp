/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */
#include "file.h"
#include "file_watcher.h"
#include <sys/inotify.h>
#include <kroll/kroll.h>

namespace ti
{
	FileWatcher::FileWatcher(const char* filepath) :
		KEventObject("Filesystem.FileWatcher"),
		readAdapter(*this, &FileWatcher::ReadThread)
	{
		notifypath = filepath;
		fd = inotify_init();
		if(fd != 0)
		{
			throw ValueException::FromFormat("inotify init failed: %s (%d)",
											  strerror(errno), errno);
		}
	}

	FileWatcher::~FileWatcher()
	{
		if(fd == 0)
		{
			int ret = 0;
			ret = inotify_rm_watch(fd, wd);
			if(ret != 0)
			{
				throw ValueException::FromFormat("inotify_rm_watch failed: %s (%d)",
												  strerror(errno), errno);
			}
			close(fd);
		}
	}

	void FileWatcher::ReadThread()
	{
		bool done=false;
		do
		{
			int len = 0;
			len = read(fd, readBuffer, INOTIFY_BUFLEN);
			if(len == -1)
			{
				done = true;
				throw ValueException::FromFormat("read() error for inotify in ReadThread(): %s (%d)",
												  strerror(errno), errno);
			} else
			{
				// need to notify caller here

			}
		} while(!done);
	}

	void FileWatcher::StartWatch(FileWatcher::EventType events)
	{
		wd = inotify_add_watch(fd, notifypath.c_str(), events);
		if(wd != 0)
		{
			throw ValueException::FromFormat("inotify_add_watch failed: %s (%d)",
											  strerror(errno), errno);
		} else
		{
			this->readThread.start(readAdapter);
		}
	}

	void FileWatcher::StopWatch()
	{
		int ret = 0;
		ret = inotify_rm_watch(fd, wd);
		if (ret != 0)
		{
			throw ValueException::FromFormat("inotify_rm_watch failed: %s (%d)",
											  strerror(errno), errno);
		}
	}
}

