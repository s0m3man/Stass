#pragma once

#include "String.hpp"

namespace Stass {
	// Represents a list of folders which can be used as part of path. The first folder is called source.
	class Path {
	public:
		// A character type which is used by OS filesystem
		using Character =
		#ifdef STASS_ENABLED_WINDOWS
			wchar_t;
		#else
			char;
		#endif

		// A string type which is used by OS filesystem
		using String = Stass::String<Character>;

		// Creates a path with root folder as source
		static Path FromRoot() {
			return Path({});
		}

		// Creates a path with disk as source
		static Path FromDisk(Character disk) {
			String DiskName = disk;

			DiskName += ':';

			return Path(DiskName);
		}

		// Constructs a path with source as given string
		Path(const String& SourceName) : Source(SourceName) {}

		// Copy constructor
		Path(const Path& Other) {
			operator=(Other);
		}

		// Destructor
		~Path() {};

		// Basic assign operator
		Path& operator =(const Path& Other) {
			Folders = Other.Folders;

			Source = Other.Source;

			return *this;
		}

		// Adds a folder to the end of the path
		Path& operator /=(const String& FolderName) {
			Folders.Add(FolderName);

			return *this;
		}

		// Adds all folders of a given path (including source) to the end of the current path
		Path& operator /=(const Path& Other) {
			Folders.Add(Other.Source);

			Folders.AddMany(Other.Folders);

			return *this;
		}

		// Returns a copy of the path with added folder at the end
		Path operator /(const String& FolderName) const {
			Path NewPath = *this;

			NewPath /= FolderName;

			return NewPath;
		}

		// Returns a copy of the path with added folders of a given path (including source) at the end
		Path operator /(const Path& Other) const {
			Path NewPath = *this;

			NewPath /= Other;

			return NewPath;
		}

		// Returns a name of the source folder
		const String& getSource() const {
			return Source;
		}

		// Returns an array of folders in the path
		const Array<String>& getFolders() const {
			return Folders;
		}

		// Returns a string representation of the path with folders separated by slashes (or backslashes)
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

		// Returns true if paths are same
		bool operator ==(const Path& Other) const {
			if (Source != Other.Source)
				return false;

			return Folders == Other.Folders;
		}

		// Returns true if paths are not same
		bool operator !=(const Path& Other) const {
			if (Source == Other.Source)
				return false;

			return Folders != Other.Folders;
		}

	private:
		// Internal folders array
		Array<String> Folders = {};

		// The source
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