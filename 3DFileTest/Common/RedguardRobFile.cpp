#include "RedguardRobFile.h"
#include "Globals.hpp"
#include <windows.h>
#include <algorithm>

using std::string;
using namespace uesp;

std::unordered_map<int, CRedguardRobFile> g_Headers;

CRedguardRobFile::CRedguardRobFile() :
	m_FileSize(0)
{

}


CRedguardRobFile::~CRedguardRobFile()
{

}


dword CRedguardRobFile::FindNextOffset(const dword Offset)
{
	return currentPosition + Offset;
}


bool CRedguardRobFile::LoadHeader(FILE* pFile)
{
	size_t BytesRead = fread(&m_Header, 1, ROB_HEADER_SIZE, pFile);
	if (BytesRead != ROB_HEADER_SIZE) return ReportError("Error: Only read %u of %u bytes of Header data from 3D file (ending at offset 0x%08lX)!", BytesRead, ROB_HEADER_SIZE, ftell(pFile));

	currentPosition = ROB_HEADER_SIZE;

	return true;
}

bool CRedguardRobFile::LoadSegmentHeader(FILE* pFile, unsigned __int32 index)
{
	if (fseek(pFile, currentPosition, SEEK_SET) != 0) return ReportError("Error: Failed to seek to offset 0x%08lX in ROB file!", currentPosition);

	size_t BytesRead = fread(&m_SegmentHeaders[index], 1, ROB_SEGMENT_HEADER_SIZE, pFile);
	if (BytesRead != ROB_SEGMENT_HEADER_SIZE) return ReportError("Error: Only read %u of %u bytes of Segment Header data from ROB file (ending at offset 0x%08lX)!", BytesRead, ROB_SEGMENT_HEADER_SIZE, ftell(pFile));

	currentPosition = currentPosition + ROB_SEGMENT_HEADER_SIZE;

	return true;
}

bool CRedguardRobFile::Generate3DFile(FILE* pFile, FILE* segmentFile, unsigned __int32 index)
{
	rg_robfile_segmentheader_t* header = &m_SegmentHeaders[index];


	if (fseek(pFile, currentPosition, SEEK_SET) != 0) return ReportError("Error: Failed to seek to offset 0x%08lX in ROB file!", currentPosition);

	byte buffer[1000];
	unsigned __int32 toWrite = header->Size;

	while (0 < toWrite) {
		size_t BytesRead = fread(buffer, sizeof(byte), min(sizeof(buffer), toWrite), pFile);
		fwrite(buffer, sizeof(byte), min(sizeof(buffer), toWrite), segmentFile);
		toWrite = toWrite - min(sizeof(buffer), toWrite);
	}

	fclose(segmentFile);

	currentPosition = currentPosition + header->Size;
	return true;
}

bool CRedguardRobFile::Load(const string Filename, const string OutputPath)
{
	FILE* pFile;
	string output = OutputPath + constants::OUTPUT_ROB_FOLDER;

	pFile = fopen(Filename.c_str(), "rb");
	if (pFile == nullptr) return ReportError("Error: Failed to open ROB file '%s' for reading!", Filename.c_str());

	m_FullName = Filename;
	m_Name = Filename;

	string folderName = ExtractFilename(Filename);
	std::replace(folderName.begin(), folderName.end(), '.', '_');

	if (!(CreateDirectory((output + folderName).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())) return ReportError("Could not create directory %s", folderName);;

	std::size_t dirPos = Filename.find_last_of("\\");
	if (dirPos != std::string::npos) m_Name = Filename.substr(dirPos + 1, Filename.length() - dirPos - 1);

	fseek(pFile, 0, SEEK_END);
	m_FileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	if (!LoadHeader(pFile))
	{
		fclose(pFile);
		return false;
	}

	m_SegmentHeaders.clear();
	m_SegmentHeaders.resize(m_Header.NumSegments * ROB_SEGMENT_HEADER_SIZE);


	for (unsigned __int32 i = 0; i < m_Header.NumSegments; i++)
	{
		if (!LoadSegmentHeader(pFile, i)) 
		{
			fclose(pFile);
			return false;
		}
		
		if (m_SegmentHeaders[i].Size > 0) {
			string segmentFileName = reinterpret_cast<const char*>(&m_SegmentHeaders[i].SegmentID1);
			string segmentFilePath = output + folderName + "\\" + segmentFileName + ".3D";
			FILE* segmentFile = fopen(segmentFilePath.c_str(), "wb");

			if (!Generate3DFile(pFile, segmentFile, i)) return ReportError("Error: Failed to generate 3D file from segment %d of %s", i, Filename.c_str());;
		}
	}

	m_FolderPath = output + folderName + "\\";

	fclose(pFile);
	return true;
}


bool CRedguardRobFile::ExportContentsAsFbx(const string Filename)
{
	return true;
}


bool CRedguardRobFile::ExportContentsAs3D(const string Filename)
{
	return true;
}


void CRedguardRobFile::ReportInfo()
{
	
}


