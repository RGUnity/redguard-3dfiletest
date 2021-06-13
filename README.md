This is a project that can be used for deciphering the files of TESA: Redguard and transforming them into a readable format. Based on Dave Humphreys 3D File Test repository. It is not 
yet ready to be used as a well designed API library or standalone command line program. Ideally the source code in the "Common" subdirectory can be copied and modified to fit your desired usage.

For basic usage, import the project, install the required library and set the command line arguments to
* Path to the location you want the parsed output files to be stored (currently, the subfolder structure will need to be created manually and you'll need to make sure to have working writing permissions for your program there)
* Path to your Redguard installation, i.e. the folder containing the fxart and maps folders (optional, if unused, the program will assume C:\Program Files (x86)\GOG Galaxy\Games\Redguard\Redguard\, which is the default for the GOG version of the game)


Basic Features:
* Export .3D/3DC files to FBX files.
* Export .ROB files to FBX files.
* Export TEXBSI.* files to PNG files.
* Export .WLD files to a readable folder format

Developed On:
* Windows 10 (Visual Studio 2019)

Libraries Used:
* Devil (included)
* Autodesk FBX SDK 2020-1 (https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-1 - Windows VS2017 version; Will need to be installed: For the project to work out of the box, it should be installed to C:\Program Files\Autodesk\FBX\FBX SDK\2020.1 or the location in the project settings should be adjusted accordingly)
	     
Special Thanks:
 * Dave Humphrey -- dave@uesp.net _for doing most of the hard work and deciphering the largest chunk of the mesh and texture data formats_
