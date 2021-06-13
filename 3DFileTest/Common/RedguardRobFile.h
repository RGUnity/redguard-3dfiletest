#pragma once

#include "RedguardCommon.h"


namespace uesp
{


	struct rg_robfile_header_t
	{
		dword UnknownOARC;		// "OARC"
		dword Unknown4;			// 00000004
		dword NumSegments;
		dword UnknownOARD;		// "OARD"
		dword UnknownID;
	};


	struct rg_robfile_segmentheader_t
	{
		dword OffsetNextSegment;
		dword SegmentID1;
		dword SegmentID2;
		dword UnknownType;		// 0 or 512
		dword Unknown1;
		dword Unknown2;
		dword Unknown3;			// 0
		dword UnknownInt1;
		dword UnknownInt2;
		dword UnknownInt3;
		dword Unknown4;			// 0
		dword Unknown5;			// 0
		dword Unknown6;			// 0
		dword UnknownInt4;
		dword UnknownInt5;
		dword UnknownInt6;
		dword UnknownInt7;
		dword UnknownInt8;
		dword UnknownInt9;
		dword Size;
	};


	class CRedguardRobFile
	{
	public:
		static const dword ROB_HEADER_SIZE = 20;
		static const dword ROB_SEGMENT_HEADER_SIZE = 80;


	public:
		rg_robfile_header_t m_Header;

		std::vector<rg_robfile_segmentheader_t> m_SegmentHeaders;

		long m_FileSize;
		int m_NumSegments;

		uint32_t currentPosition = 0;

		std::string m_FullName;
		std::string m_Name;
		std::string m_FolderPath;


	protected:
		dword FindNextOffset(const dword Offset);

		bool LoadHeader(FILE* pFile);
		bool LoadSegmentHeader(FILE* pFile, unsigned __int32 index);
		bool Generate3DFile(FILE* pFile, FILE* segmentFile, unsigned __int32 index);


	public:
		CRedguardRobFile();
		~CRedguardRobFile();

		bool Load(const std::string Filename, const std::string outputPath);

		bool ExportContentsAsFbx(const std::string Filename);
		bool ExportContentsAs3D(const std::string Filename);

		static void ReportInfo();

	};


	static_assert(sizeof(rg_robfile_header_t) == CRedguardRobFile::ROB_HEADER_SIZE, "Redguard ROB header struct is not the correct size!");
	//static_assert(sizeof(rg_robfile_segmentheader_t) == CRedguardRobFile::ROB_SEGMENT_HEADER_SIZE, "Redguard ROB segment header struct is not the correct size!");

};
