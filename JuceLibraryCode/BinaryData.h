/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   PoppinsBlack_ttf;
    const int            PoppinsBlack_ttfSize = 151340;

    extern const char*   PoppinsBold_ttf;
    const int            PoppinsBold_ttfSize = 153900;

    extern const char*   PoppinsExtraBold_ttf;
    const int            PoppinsExtraBold_ttfSize = 152712;

    extern const char*   PoppinsExtraLight_ttf;
    const int            PoppinsExtraLight_ttfSize = 161404;

    extern const char*   PoppinsLight_ttf;
    const int            PoppinsLight_ttfSize = 159848;

    extern const char*   PoppinsMedium_ttf;
    const int            PoppinsMedium_ttfSize = 156480;

    extern const char*   PoppinsRegular_ttf;
    const int            PoppinsRegular_ttfSize = 158192;

    extern const char*   PoppinsSemiBold_ttf;
    const int            PoppinsSemiBold_ttfSize = 155192;

    extern const char*   PoppinsThin_ttf;
    const int            PoppinsThin_ttfSize = 161600;

    extern const char*   SedgwickAveDisplayRegular_ttf;
    const int            SedgwickAveDisplayRegular_ttfSize = 135996;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 10;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
