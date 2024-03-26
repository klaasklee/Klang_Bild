/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   bgControlBar_2_png;
    const int            bgControlBar_2_pngSize = 6933409;

    extern const char*   bgLayerControlAndWave_2_png;
    const int            bgLayerControlAndWave_2_pngSize = 2760223;

    extern const char*   bgLayerLabel_png;
    const int            bgLayerLabel_pngSize = 562141;

    extern const char*   bgLayersContainer_2_png;
    const int            bgLayersContainer_2_pngSize = 20687992;

    extern const char*   bgPlayHeadRuler_2_png;
    const int            bgPlayHeadRuler_2_pngSize = 2235493;

    extern const char*   LevelMeterGrill_svg;
    const int            LevelMeterGrill_svgSize = 16264;

    extern const char*   logo_2_svg;
    const int            logo_2_svgSize = 25751;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

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
