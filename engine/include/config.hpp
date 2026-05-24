#pragma once

/*
    Pass ENGINE_DEBUG, BOARD_DEBUG or MOVEGEN_DEBUG into the
    CMake target compile definitions to enable any of these options

        e.g.
        target_compile_definitions(chesstests PRIVATE
                ENGINE_DEBUG, BOARD_DEBUG, MOVEGEN_DEBUG
        )

        Within the hot paths these are using
            if constexpr(config::engine_debug)
        So this isn't affected within release builds
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