#include "clutils.h"

const char* clErrorString(cl_int err) {
	switch (err) {
		case CL_SUCCESS                         : return "Success";
		case CL_DEVICE_NOT_FOUND                : return "Device not found";
		case CL_DEVICE_NOT_AVAILABLE            : return "Device not available";
		case CL_COMPILER_NOT_AVAILABLE          : return "Compiler not available";
		case CL_MEM_OBJECT_ALLOCATION_FAILURE   : return "Memory object allocation failure";
		case CL_OUT_OF_RESOURCES                : return "Out of resources";
		case CL_OUT_OF_HOST_MEMORY              : return "Out of host memory";
		case CL_PROFILING_INFO_NOT_AVAILABLE    : return "Profiling info not available";
		case CL_MEM_COPY_OVERLAP                : return "Memory copy overlap";
		case CL_IMAGE_FORMAT_MISMATCH           : return "Image format mismatch";
		case CL_IMAGE_FORMAT_NOT_SUPPORTED      : return "Image format not supported";
		case CL_BUILD_PROGRAM_FAILURE           : return "Build program failure";
		case CL_MAP_FAILURE                     : return "Map failure";
		case CL_INVALID_VALUE                   : return "Invalid value";
		case CL_INVALID_DEVICE_TYPE             : return "Invalid device type";
		case CL_INVALID_PLATFORM                : return "Invalid platform";
		case CL_INVALID_DEVICE                  : return "Invalid device";
		case CL_INVALID_CONTEXT                 : return "Invalid context";
		case CL_INVALID_QUEUE_PROPERTIES        : return "Invalid queue properties";
		case CL_INVALID_COMMAND_QUEUE           : return "Invalid command queue";
		case CL_INVALID_HOST_PTR                : return "Invalid host pointer";
		case CL_INVALID_MEM_OBJECT              : return "Invalid memory object";
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR : return "Invalid image format descriptor";
		case CL_INVALID_IMAGE_SIZE              : return "Invalid image size";
		case CL_INVALID_SAMPLER                 : return "Invalid sampler";
		case CL_INVALID_BINARY                  : return "Invalid binary";
		case CL_INVALID_BUILD_OPTIONS           : return "Invalid build options";
		case CL_INVALID_PROGRAM                 : return "Invalid program";
		case CL_INVALID_PROGRAM_EXECUTABLE      : return "Invalid program executable";
		case CL_INVALID_KERNEL_NAME             : return "Invalid kernel name";
		case CL_INVALID_KERNEL_DEFINITION       : return "Invalid kernel definition";
		case CL_INVALID_KERNEL                  : return "Invalid kernel";
		case CL_INVALID_ARG_INDEX               : return "Invalid argument index";
		case CL_INVALID_ARG_VALUE               : return "Invalid argument value";
		case CL_INVALID_ARG_SIZE                : return "Invalid argument size";
		case CL_INVALID_KERNEL_ARGS             : return "Invalid kernel arguments";
		case CL_INVALID_WORK_DIMENSION          : return "Invalid work dimension";
		case CL_INVALID_WORK_GROUP_SIZE         : return "Invalid work group size";
		case CL_INVALID_WORK_ITEM_SIZE          : return "invalid work item size";
		case CL_INVALID_GLOBAL_OFFSET           : return "Invalid global offset";
		case CL_INVALID_EVENT_WAIT_LIST         : return "Invalid event wait list";
		case CL_INVALID_EVENT                   : return "Invalid event";
		case CL_INVALID_OPERATION               : return "Invalid operation";
		case CL_INVALID_GL_OBJECT               : return "Invalid OpenGL object";
		case CL_INVALID_BUFFER_SIZE             : return "Invalid buffer size";
		case CL_INVALID_MIP_LEVEL               : return "Invalid MIP level";
	}
	return "UNKNOWN ERROR";
}

OpenCLData clFullSetup(const char* kernel_source, const char* entrypoint) {
	int rc;
	OpenCLData data = {};

	eset(clGetPlatformIDs(1, &data.platform_id, NULL));
	eset(clGetDeviceIDs(data.platform_id, CL_DEVICE_TYPE_GPU, 1, &data.device_id, NULL));
	eref(data.context = clCreateContext(NULL, 1, &data.device_id, NULL, NULL, &rc));
	eref(data.queue   = clCreateCommandQueueWithProperties(data.context, data.device_id, NULL, &rc));
	eref(data.program = clCreateProgramWithSource(data.context, 1, &kernel_source, NULL, &rc));

	if(clBuildProgram(data.program, 0, NULL, NULL, NULL, NULL) != CL_SUCCESS) {
		char build_log[4096];
		eset(clGetProgramBuildInfo(data.program, data.device_id, CL_PROGRAM_BUILD_LOG, 4096, build_log, NULL));
		errx(1, "Build failure:\n%s", build_log);
	}

	eref(data.kernel  = clCreateKernel(data.program, entrypoint, &rc));

	return data;
}

void clFullCleanup(OpenCLData data) {
	int rc;
	eset(clReleaseKernel(data.kernel));
	eset(clReleaseProgram(data.program));
	eset(clReleaseCommandQueue(data.queue));
	eset(clReleaseContext(data.context));
}
