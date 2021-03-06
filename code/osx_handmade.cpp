///////////////////////////////////////////////////////////////////////
// osx_main.mm
//
// Jeff Buck
// Copyright 2014-2016. All Rights Reserved.
//

/*
	TODO(jeff): THIS IS NOT A FINAL PLATFORM LAYER!!!

	This will be updated to keep parity with Casey's win32 platform layer.
	See his win32_handmade.cpp file for TODO details.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <libproc.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <mach/mach_init.h>
#include <mach/mach_time.h>
#include <mach/vm_map.h>
#include <mach-o/dyld.h>
#include <libkern/OSAtomic.h> // for OSMemoryBarrier
#include <pthread.h>

#define GL_GLEXT_LEGACY

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
//#include <OpenGL/glext.h>
//#include <OpenGL/glu.h>

#include "handmade_platform.h"


#define GL_BGRA_EXT                                          0x80E1

typedef void gl_bind_framebuffer(GLenum target, GLuint framebuffer);
typedef void gl_gen_framebuffers(GLsizei n, GLuint *framebuffers);
typedef void gl_framebuffer_texture_2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLenum gl_check_framebuffer_status(GLenum target);

global_variable gl_bind_framebuffer* glBindFramebuffer;
global_variable gl_gen_framebuffers* glGenFramebuffers;
global_variable gl_framebuffer_texture_2D* glFramebufferTexture2D;
global_variable gl_check_framebuffer_status* glCheckFramebufferStatus;




//#include "handmade_intrinsics.h"
//#include "handmade_math.h"
#include "handmade_shared.h"


#include "osx_handmade.h"

platform_api Platform;


global_variable b32 GlobalSoftwareRendering;
global_variable b32 GlobalRunning = 1;
global_variable b32 GlobalPause;
global_variable b32 GlobalShowSortGroups;

global_variable GLuint OpenGLDefaultInternalTextureFormat;



// NOTE(jeff): These are already defined in glext.h.
#undef GL_EXT_texture_sRGB
#undef GL_EXT_framebuffer_sRGB

#include "handmade_render.h"
#include "handmade_sort.cpp"
#include "handmade_opengl.cpp"
#include "handmade_render.cpp"

#include "osx_handmade_debug.cpp"
#include "osx_handmade_file.cpp"
#include "osx_handmade_process.cpp"
#include "osx_handmade_thread.cpp"
#include "osx_handmade_audio.cpp"
#include "osx_handmade_hid.cpp"
#include "osx_handmade_dylib.cpp"
#include "osx_handmade_playback.cpp"
#include "osx_handmade_game.cpp"


#if 0
// NOTE(jeff): vvvv Workaround hacks to avoid including handmade.h here.
// If we include it, we will get duplicate symbol linker errors
// for PushSize_().
#ifndef ArrayCount
#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#endif
// NOTE(jeff): ^^^^ End of workaround hacks.
#endif


//#define PLATFORM_ALLOCATE_MEMORY(name) void *name(memory_index Size)
PLATFORM_ALLOCATE_MEMORY(OSXAllocateMemory)
{
	void* Result = malloc(Size);

	return(Result);
}


//#define PLATFORM_DEALLOCATE_MEMORY(name) void name(void *Memory)
PLATFORM_DEALLOCATE_MEMORY(OSXDeallocateMemory)
{
	if (Memory)
	{
		free(Memory);
	}
}


