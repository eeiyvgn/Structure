#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)

struct FileRecrodHeaderFormat
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

#pragma pack(pop)

using namespace std;

int main()
{
    ifstream dat_file("File_record.dat", ios::binary);

    if(dat_file.is_open())
    {
        dat_file.seekg(0, ios::end);
        int fileSize = dat_file.tellg();
        dat_file.seekg(0, ios::beg);

        vector<char> dat_data(fileSize, 0);

        dat_file.read(dat_data.data(), fileSize);

        FileRecrodHeaderFormat* frhf = reinterpret_cast<FileRecrodHeaderFormat*>(&dat_data[0]);

        cout << "Text string: " << hex << int(frhf->TextString) << endl;
        cout << "Offset of the first attribute: " << hex << int(frhf->OffsetOfTheFirstAttribute) << endl;
        cout << "File entry number: " << hex << int(frhf->FileEntryNumber) << endl;
    }

    return 0;
}
