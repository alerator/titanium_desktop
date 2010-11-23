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
	FileWatcher::FileWatcher(const char* filepath)
	{
		notifypath = filepath;
		fd = inotify_init();
		if(fd != 0)
		{
			throw ValueException::FromString("inotify init failed");
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
				throw ValueException::FromString("inotify_rm_watch failed");
			}
			close(fd);
		}
	}

	void FileWatcher::start(FileWatcher::EventType events)
	{
		wd = inotify_add_watch(fd, notifypath.c_str(), events);
		if(wd != 0)
		{
			throw ValueException::FromString("inotify_add_watch failed");
		}
	}

	void FileWatcher::stop()
	{
		int ret = 0;
		ret = inotify_rm_watch(fd, wd);
		if (ret != 0)
		{
			throw ValueException::FromString("inotify_rm_watch failed");
		}
	}
}

