#pragma once

#include "String.hpp"

namespace Stass {
	class Path {
	public:
		using Character =
		#ifdef STASS_ENABLED_WINDOWS
			wchar_t;
		#else
			char;
		#endif

		using String = Stass::String<Character>;

		static Path FromRoot() {
			return Path({});
		}

		static Path FromDisk(Character disk) {
			String DiskName = disk;

			DiskName += ':';

			return Path(DiskName);
		}

		Path(const String& SourceName) : Source(SourceName) {}

		Path(const Path& Other) {
			operator=(Other);
		}

		~Path() {};

		Path& operator =(const Path& Other) {
			Folders = Other.Folders;

			Source = Other.Source;

			return *this;
		}

		Path& operator /=(const String& FolderName) {
			Folders.Add(FolderName);

			return *this;
		}

		Path& operator /=(const Path& Other) {
			Folders.Add(Other.Source);

			Folders.AddMany(Other.Folders);

			return *this;
		}

		Path operator /(const String& FolderName) const {
			Path NewPath = *this;

			NewPath /= FolderName;

			return NewPath;
		}

		Path operator /(const Path& Other) const {
			Path NewPath = *this;

			NewPath /= Other;

			return NewPath;
		}

		const String& getSource() const {
			return Source;
		}

		const Array<String>& getFolders() const {
			return Folders;
		}

		String ToString(bool windowsStyle = enabledWindows) const {
			String NewStr = Source;

			if (windowsStyle)
			{
				for (const String& Folder : Folders) {
					NewStr += '\\';

					NewStr += Folder;
				}

				NewStr += '\\';

				return NewStr;
			}

			for (const String& Folder : Folders) {
				NewStr += '/';

				NewStr += Folder;
			}

			NewStr += '/';

			return NewStr;
		}

	private:
		Array<String> Folders = {};

		String Source;
	};

	class File {
	public:
		File(const Path& FilePath, const Path::String& FileName) : PathToFile(FilePath), Name(FileName) {};

		~File() {};

		const Path& getPath() const {
			return PathToFile;
		}

		const Path::String& getName() const {
			return Name;
		}

		Path::String GetFullPath(bool windowsStyle = enabledWindows) const {
			return PathToFile.ToString(windowsStyle) + Name;
		}

	private:
		Path PathToFile;

		Path::String Name;
	};
}