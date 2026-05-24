#pragma once

/*
    In CMakeLists.txt each separate executable library is compiled with different symbols
    So that this file can be targetted with a different config

    Within the engine and hot paths, the configs are treated with if constexpr
    so that there isn't any performance drops for the release builds
 */


namespace config {

#ifdef ENGINE_DEBUG
    inline constexpr bool engine_debug = true;
#else
    inline constexpr bool engine_debug = false;
#endif

#ifdef BOARD_DEBUG
    inline constexpr bool board_debug = true;
#else
    inline constexpr bool board_debug = false;
#endif

#ifdef MOVEGEN_DEBUG
    inline constexpr bool movegen_debug = true;
#else
    inline constexpr bool movegen_debug = false;
#endif

}