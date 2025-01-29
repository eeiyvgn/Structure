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
    cout << "Hello world!" << endl;
    return 0;
}
