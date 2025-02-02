#include <iostream>
#include <fstream>
#include <vector>
#include <locale>
#include <windows.h>
#include <string>
#include <cstdint>

using namespace std;

#pragma pack(push, 1)

struct FileRecordHeaderFormat
{
    uint32_t TextString;
    uint16_t OffsetOfTheFileRecord;
    uint16_t SizeOfTheFileRecord;
    uint64_t SequenceNumber;
    uint16_t SequantialSectorNumber;
    uint16_t NumberOfFileReferences;
    uint16_t OffsetOfTheFirstAttribute;
    uint16_t UsageAndDirectoryFlags;
    uint32_t ActualSizeOfTheFileEntry;
    uint32_t AllocatedFileRecordSize;
    uint64_t FileReference;
    uint32_t ID;
    uint32_t FileEntryNumber;
};

struct AttributeHeaderFormat
{
    uint32_t AttributeType;
    uint32_t AttributeLength;
    uint8_t NonResidentAttributeFlag;
    uint8_t AttributeNameLength;
    uint16_t AttributeNameOffset;
    uint16_t AttributePackingFlag;
    uint16_t AttributeID;
    uint32_t SizeOfAttributeContentInBytes;
    uint16_t OffsetOfAttributeContent;
    uint8_t IndexingFlag;
    uint8_t Filling;
};

struct FormatAttributeDataArea
{
    uint64_t FileLink;
    uint64_t FileCreationTime;
    uint64_t TimeLastFileChange;
    uint64_t TimeLastFileRecordChange;
    uint64_t TimeLastFileAccess;
    uint64_t DedicatedFileSize;
    uint64_t RealFileSize;
    uint64_t Flags;
    uint8_t FileNameLength;
    uint8_t FileNameSpaceCodeFlags;
    uint16_t FileName[];
};

#pragma pack(pop)

int main()
{
    setlocale(LC_ALL, "Russian");

    ifstream dat_file("File_record.dat", ios::binary);

    if(dat_file.is_open())
    {
        dat_file.seekg(0, ios::end);
        int fileSize = dat_file.tellg();
        dat_file.seekg(0, ios::beg);

        vector<char> dat_data(fileSize, 0);

        dat_file.read(dat_data.data(), fileSize);

        FileRecordHeaderFormat* frhf = reinterpret_cast<FileRecordHeaderFormat*>(&dat_data[0]);

        cout << "Text string: " << hex << int(frhf->TextString) << endl;
        cout << "Offset of the first attribute: " << hex << int(frhf->OffsetOfTheFirstAttribute) << endl;
        cout << "File entry number: " << hex << int(frhf->FileEntryNumber) << endl;

        size_t offset  = frhf->OffsetOfTheFirstAttribute;

        while (offset + sizeof(AttributeHeaderFormat) < fileSize)
        {
            AttributeHeaderFormat* ahf = reinterpret_cast<AttributeHeaderFormat*>(&dat_data[offset]);

            if (ahf->AttributeType != 0xFFFFFFFF)
            {
                cout << "Attribute name: ";

                switch (ahf->AttributeType)
                {
                case 0x10: cout << "$STANDART_INFORMATION" << endl; break;
                case 0x20: cout << "$ATTRIBUTE_LIST" << endl; break;
                case 0x30: cout << "$FILE_NAME" << endl; break;
                case 0x40: cout << "$OBJECT_ID" << endl; break;
                case 0x50: cout << "$SECURITY_DESCRIPTOR" << endl; break;
                case 0x80: cout << "$DATA" << endl; break;
                case 0x90: cout << "$INDEX_ROOT" << endl; break;
                case 0xA0: cout << "$INDEX_ALLOCATION" << endl; break;
                case 0xB0: cout << "$BITMAP" << endl; break;
                case 0x100: cout << "$LOGGED_UTILITY_STREAM" << endl; break;
                }

                cout << "Attribute type: 0x"<< hex << ahf->AttributeType << endl;
            }

            if (ahf->AttributeType == 0x30)
            {
                uint8_t FileNameLength = dat_data[offset + 0x58];

                const wchar_t* FileName = reinterpret_cast<const wchar_t*>(&dat_data[offset + 0x5A]);

                wstring name (FileName, FileNameLength);

                wcout << L"File name: " << name << endl;
            }

            if (ahf->AttributeType == 0x80)
            {
                uint16_t OffsetOfAttributeContent = offset + ahf->OffsetOfAttributeContent;

                uint32_t SizeOfAttributeContentInBytes = ahf->SizeOfAttributeContentInBytes;

                const char* FileData = reinterpret_cast<const char*>(&dat_data[OffsetOfAttributeContent]);

                string utf8 (FileData, SizeOfAttributeContentInBytes);

                vector<wchar_t> utf16 (SizeOfAttributeContentInBytes);

                MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), SizeOfAttributeContentInBytes, utf16.data(), utf16.size());

                wstring data(utf16.begin(), utf16.end());

                wcout << L"File data: " << data << endl;

            }

            offset += ahf->AttributeLength;
        }
    }

    return 0;
}
