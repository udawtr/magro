SUBDIRS = distributions

bin_PROGRAMS = magro
 
magro_SOURCES = conf.h compiler.h main.c env.h env.c \
compiler.c node.h node.c model.h model.c \
chain.h chain.c \
sampler.h sampler.c norm_sampler.c gamma_sampler.c real_sampler.c \
distribution.h distribution.c \
function_node.c stochastic_node.c array_node.c symbol_node.c constant_node.c range_node.c

magro_LDADD = \
${top_builddir}/src/nmath/libnmath.a \
${top_builddir}/src/mainapp/distributions/libdist.a \
${top_builddir}/src/parser_rdata/libparser_rdata.a \
${top_builddir}/src/parser_bugs/libparser_bugs.a \
${top_builddir}/src/clearsilver/libcs.a


