/*
 *   cod - T1
 *     
 *       This test is relatively simple.  Local ints, int parameters,
 *     simple struct parameters, structs with internal static arrays.
 */
#include "config.h"
#include "data_funcs.h"
#include "cod.h"
#include "assert.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int verbose = 0;
#ifdef NO_EMULATION
#define GEN_PARSE_CONTEXT(x) \
x = new_cod_parse_context();
#define EC_param0
#define EC_param1
#else
#define GEN_PARSE_CONTEXT(x) \
x = new_cod_parse_context();\
cod_add_param("ec", "cod_exec_context", 0, x);
#define EC_param0 ec
#define EC_param1 ec,
#endif

extern void
write_buffer(char *filename, FMStructDescList desc, void *data, 
             int test_num);
extern char *read_buffer(FMContext c, char *read_file, int test_num);

static int *
dummy(int *p)
{
  return p;
}

int
main(int argc, char**argv)
{
    int test_num = 0;
    int run_only = -1;
    char *read_file = NULL;
    char *write_file = NULL;
    while (argc > 1) {
	if (strcmp(argv[1], "-v") == 0) {
	    verbose++;
	} else if (strcmp(argv[1], "-w") == 0) {
	    if (argc <= 1) {
		printf("Need argument to \"-w\"\n");
	    } else {
		write_file = strdup(argv[2]);
	    }
	    argc--; argv++;
	} else if (strcmp(argv[1], "-r") == 0) {
	    if (argc <= 1) {
		printf("Need argument to \"-r\"\n");
	    } else {
		read_file = strdup(argv[2]);
	    }
	    argc--; argv++;
	} else if (strcmp(argv[1], "-o") == 0) {
	    sscanf(argv[2], "%d", &run_only);
	    argc--; argv++;
	}
	argc--; argv++;
    }
    if ((run_only == -1) || (run_only == test_num)) {
	/* test the basics */
	static char extern_string[] = "int printf(string format, ...);";
	static cod_extern_entry externs[] = 
	{
	    {"printf", (void*)(long)printf},
	    {(void*)0, (void*)0}
	};

	char code_string[] = "\n\
{\n\
    timeval t1, t2;\n			\
    int delay = 10000;\n\
    long accum = 0;\n\
    long usec_duration;\n\
    gettimeofday(&t1);\n\
\n\
    if (t1.tv_sec < 1423159919) { /* in the past */ \n\
       return 0;\n\
    }\n\
    while(delay > 0) {  /* just so there's a duration */\n	\
	delay--;\n\
        accum++;\n\
    }\n\
    gettimeofday(&t2);\n\
    usec_duration = (t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec;\n\
\n\
    if ((usec_duration < 1) || (usec_duration > 1000000)) {\n\
       printf(\"duration is %d usecs, unreasonable\\n\", usec_duration);\n    \
       return 0; /* seems unreasonable */\n\
    }\n\
    return 1;\n\
}";

	cod_parse_context context;
	cod_exec_context ec;
	cod_code gen_code;
	long (*func)();
	long result;

	GEN_PARSE_CONTEXT(context);
	cod_assoc_externs(context, externs);
	cod_parse_for_context(extern_string, context);
	gen_code = cod_code_gen(code_string, context);
	ec = cod_create_exec_context(gen_code);
	func = (long(*)()) (long) gen_code->func;
	if (verbose) cod_dump(gen_code);
	result = func(EC_param0);
	assert(result == 1);
	cod_code_free(gen_code);
	cod_exec_context_free(ec);
	cod_free_parse_context(context);
    }
    test_num++; /* 1 */
    if ((run_only == -1) || (run_only == test_num)) {
	/* test the basics */
	char code_string[] = "{\n\
    int j = 2;\n\
    int i = 3;\n\
    i = !j;\n\
    return i;\n\
}";

	cod_parse_context context;
	cod_exec_context ec;
	cod_code gen_code;
	long (*func)();
	long result;

	GEN_PARSE_CONTEXT(context);
	gen_code = cod_code_gen(code_string, context);
	ec = cod_create_exec_context(gen_code);
	func = (long(*)()) (long) gen_code->func;
	if (verbose) cod_dump(gen_code);
	result = func(EC_param0);
	assert(result == (!2));
	cod_exec_context_free(ec);
	cod_code_free(gen_code);
	cod_free_parse_context(context);
    }
    test_num++; /* 2 */
    if ((run_only == -1) || (run_only == test_num)) {

	/* test the ability to have a parameter */
	char code_string[] = "{\n\
    int j = 4;\n\
    long k = 10;\n\
    short l = 3;\n\
\n\
    return l * (j + k + i);\n\
}";

	cod_parse_context context = new_cod_parse_context();
	cod_exec_context ec;
	cod_code gen_code;
    	long (*func)();

#ifdef NO_EMULATION
	cod_subroutine_declaration("int proc(int i)", context);
#else
	cod_subroutine_declaration("int proc(cod_exec_context ec, int i)", context);
#endif
	gen_code = cod_code_gen(code_string, context);
	ec = cod_create_exec_context(gen_code);
	func = (long(*)()) (long) gen_code->func;
	if (verbose) cod_dump(gen_code);
        assert(func(EC_param1 15) == 87);
	cod_exec_context_free(ec);
	cod_code_free(gen_code);
	cod_free_parse_context(context);
    }
    test_num++; /* 3 */
    if ((run_only == -1) || (run_only == test_num)) {
	/* structured types */
	char code_string[] = "{\n\
    input.j = 5;\n\
    return input.l * (input.j + input.k + input.i);\n\
}";

	typedef struct test {
	    int i;
	    int j;
	    long k;
	    short l;
	} test_struct, *test_struct_p;

	static char extern_string[] = "int printf(string format, ...);";
	static cod_extern_entry externs[] = 
	{
	    {"printf", (void*)(long)printf},
	    {(void*)0, (void*)0}
	};
	FMField struct_fields[] = {
	    {"i", "integer", sizeof(int), FMOffset(test_struct_p, i)},
	    {"j", "integer", sizeof(int), FMOffset(test_struct_p, j)},
	    {"k", "integer", sizeof(long), FMOffset(test_struct_p, k)},
	    {"l", "integer", sizeof(short), FMOffset(test_struct_p, l)},
	    {(void*)0, (void*)0, 0, 0}};

	cod_parse_context context = new_cod_parse_context();
	cod_exec_context ec;
	test_struct str;
	test_struct *param = &str;	
	cod_code gen_code;
	long (*func)();

	cod_assoc_externs(context, externs);
	cod_parse_for_context(extern_string, context);
	if (read_file) {
	    FMContext c = create_local_FMcontext();
	    char *buf = read_buffer(c, read_file, test_num);
	    param = (test_struct *)buf;
#ifdef NO_EMULATION
	    cod_add_encoded_param("input", buf, 0, c, context);
#else
	    cod_add_param("ec", "cod_exec_context", 0, context);
	    cod_add_encoded_param("input", buf, 1, c, context);
#endif
	} else {
	    cod_add_simple_struct_type("struct_type", struct_fields, context);
#ifdef NO_EMULATION
	    cod_subroutine_declaration("int proc(struct_type *input)", context);
#else
	    cod_subroutine_declaration("int proc(cod_exec_context ec, struct_type *input)", context);
#endif
	}
	gen_code = cod_code_gen(code_string, context);
	ec = cod_create_exec_context(gen_code);
	func = (long(*)(test_struct_p)) (long) gen_code->func;
	if (verbose) cod_dump(gen_code);

	str.i = 15;
	str.j = 4;
	str.k = 10;
	str.l = 3;
	assert(func(EC_param1 param) == 90);
	cod_exec_context_free(ec);
	cod_code_free(gen_code);
	cod_free_parse_context(context);
	if (write_file) {
	    FMStructDescRec formats[] = {{"struct", struct_fields, sizeof(str), NULL},{NULL, NULL, 0, NULL}};
	    write_buffer(write_file, &formats[0], &str, test_num);
	}
    }
    test_num++; /* 4 */
    if ((run_only == -1) || (run_only == test_num)) {
	static char extern_string[] = "int printf(string format, ...);";
	static cod_extern_entry externs[] = 
	{
	    {"printf", (void*)(long)printf},
	    {(void*)0, (void*)0}
	};
	static char code[] = "{\
		    int i, j;\n\
		    double sum = 0.0;\n\
		    double average = 0.0;\n\
		    for(i = 0; i<37; i= i+1) {\n\
		        for(j = 0; j<253; j=j+1) {\n\
			sum = sum + input.levels[j][i];\n\
		        }\n\
		    }\n\
		    average = sum / (37 * 253);\n\
		    return average;\n\
		}";

	static FMField input_field_list[] =
	{
	    {"levels", "float[253][37]", sizeof(double), 0},
	    {(void*)0, (void*)0, 0, 0}
	};

	cod_parse_context context = new_cod_parse_context();
	cod_exec_context ec;
	int i, j;
	double levels[253][37];
	cod_code gen_code;
	double (*func)(), result;
	double *param = &levels[0][0];

	cod_assoc_externs(context, externs);
	cod_parse_for_context(extern_string, context);

	if (read_file) {
	    FMContext c = create_local_FMcontext();
	    char *buf = read_buffer(c, read_file, test_num);
	    param = (double*)buf;
#ifdef NO_EMULATION
	    cod_add_encoded_param("input", buf, 0, c, context);
#else
	    cod_add_param("ec", "cod_exec_context", 0, context);
	    cod_add_encoded_param("input", buf, 1, c, context);
#endif
	    cod_set_return_type("double", context);

	} else {
	    cod_add_simple_struct_type("input_type", input_field_list, context);
#ifdef NO_EMULATION
	    cod_subroutine_declaration("double proc(input_type *input)", context);
#else
	    cod_subroutine_declaration("double proc(cod_exec_context ec, input_type *input)", context);
#endif
	}
	for(i=0; i< 253; i++) {
	    for (j=0; j< 37; j++) {
	        levels[i][j] = i + 1000*j;
	    }
	}

	gen_code = cod_code_gen(code, context);
	ec = cod_create_exec_context(gen_code);
	func = (double (*)())(long) gen_code->func;
	if (verbose) cod_dump(gen_code);
	result = func(EC_param1 param);
	if (result != 18126.00) {
	    printf("Got %e from double float array sum, expected 18126.00\n", result);
	    exit(1);
	}
	if (write_file) {
	    FMStructDescRec formats[] = {{"level_struct", input_field_list, sizeof(levels), NULL},
					 {NULL, NULL, 0, NULL}};
	    write_buffer(write_file, &formats[0], &levels, test_num);
	}
	cod_exec_context_free(ec);
	cod_code_free(gen_code);
	cod_free_parse_context(context);
    }
    test_num++; /* 5 */
    if ((run_only == -1) || (run_only == test_num)) {
	static char extern_string[] = "int printf(string format, ...);\n\
int *dummy(int*p);";
	static cod_extern_entry externs[] = 
	{
	    {"printf", (void*)(long)printf},
	    {"dummy", (void*)(long)dummy},
	    {(void*)0, (void*)0}
	};
	typedef struct test {
	    int i;
	    double levels[3];
	} test_struct, *test_struct_p;

	static char code[] = "{\
		return dummy(input.levels);\n\
		}";

	static FMField input_field_list[] =
	{
	    {"i", "integer", sizeof(int), 0},
	    {"levels", "float[3]", sizeof(double), FMOffset(test_struct_p, levels)},
	    {(void*)0, (void*)0, 0, 0}
	};

	cod_parse_context context = new_cod_parse_context();
	cod_exec_context ec;
	int i, j = 1;
	double levels;
	cod_code gen_code;
	int *(*func)(), *result;
	test_struct strct;
	test_struct *param = &strct;

	cod_assoc_externs(context, externs);
	cod_parse_for_context(extern_string, context);

	if (read_file) {
	    FMContext c = create_local_FMcontext();
	    char *buf = read_buffer(c, read_file, test_num);
	    param = (test_struct*)buf;
#ifdef NO_EMULATION
	    cod_add_encoded_param("input", buf, 0, c, context);
#else
	    cod_add_param("ec", "cod_exec_context", 0, context);
	    cod_add_encoded_param("input", buf, 1, c, context);
#endif
	    cod_set_return_type("int*", context);

	} else {
	    cod_add_simple_struct_type("input_type", input_field_list, context);
#ifdef NO_EMULATION
	    cod_subroutine_declaration("int * proc(input_type *input)", context);
#else
	    cod_subroutine_declaration("int * proc(cod_exec_context ec, input_type *input)", context);
#endif
	}
	for(i=0; i< 3; i++) {
	    strct.levels[i] = i + 1000*j;
	}

	gen_code = cod_code_gen(code, context);
	ec = cod_create_exec_context(gen_code);
	func = (int * (*)())(long) gen_code->func;
	if (verbose) cod_dump(gen_code);
	result = func(EC_param1 param);
	if (result != (int*)&strct.levels) {
	    printf("Got %p from address of array, expected %p\n", result, &strct.levels);
	    exit(1);
	}
	if (write_file) {
	    FMStructDescRec formats[] = {{"level_struct", input_field_list, sizeof(levels), NULL},
					 {NULL, NULL, 0, NULL}};
	    write_buffer(write_file, &formats[0], &levels, test_num);
	}
	cod_exec_context_free(ec);
	cod_code_free(gen_code);
	cod_free_parse_context(context);
    }

    return 0;
}
