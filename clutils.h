/*
  OpenCL Utility functions
 */

#ifndef CLUTILS_H_INC
#define CLUTILS_H_INC

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include <stdio.h>
#include <err.h>

#define QUOTE(x) #x
#define STR(x) QUOTE(x)
#define FILE_LINE __FILE__ ":" STR(__LINE__)
#define ERROR errx(rc, "error: %s: %s", FILE_LINE, clErrorString(rc));
#define eset(x) if((rc = x) != CL_SUCCESS) ERROR
#define eref(x) x; if(rc != CL_SUCCESS) ERROR

typedef struct OpenCLData {
	cl_platform_id   platform_id;
	cl_device_id     device_id;
	cl_context       context;
	cl_command_queue queue;
	cl_program       program;
	cl_kernel        kernel;

} OpenCLData;

const char* clErrorString(cl_int err);
OpenCLData  clFullSetup(const char* kernel_source, const char* entrypoint);
void        clFullCleanup(OpenCLData data);

#endif /* CLUTILS_H_INC */
