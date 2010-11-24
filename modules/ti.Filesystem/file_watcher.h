/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef _FILE_WATCHER_H_
#define _FILE_WATCHER_H_

#include <sys/inotify.h>
#include <string>
#include <errno.h>

#define INOTIFY_EVENT_SIZE (sizeof(struct inotify_event))
#define INOTIFY_BUFLEN (1024 * (INOTIFY_EVENT_SIZE + 16))

namespace ti
{
	class FileWatcher : public KEventObject 
	{
	public:
		enum EventType 
		{
		   ACCESS = IN_ACCESS,
           ATTRIB = IN_ATTRIB,
           CLOSE_WRITE = IN_CLOSE_WRITE,
           CLOSE_NOWRITE = IN_CLOSE_NOWRITE,
           CREATE = IN_CREATE,
           DELETE = IN_DELETE,
           DELETE_SELF = IN_DELETE_SELF,
           MODIFY = IN_MODIFY,
           MOVE_SELF = IN_MOVE_SELF,
           MOVED_FROM = IN_MOVED_FROM,
           MOVED_TO = IN_MOVED_TO,
           OPEN = IN_OPEN,
		   ALL_EVENTS = IN_ALL_EVENTS
		};
		FileWatcher(const char* filepath);
		virtual ~FileWatcher();

		void StartWatch(FileWatcher::EventType event);
		void StopWatch();
		
	private:
		std::string notifypath;
		int fd;			// file descriptor for inotify
		int wd;			// watch descriptor for inotify
		unsigned char readBuffer[INOTIFY_BUFLEN];

		void ReadThread();

		Poco::RunnableAdapter<FileWatcher> readAdapter;
		Poco::Thread readThread;


		EventType notifyevents;
	};
}

#endif

