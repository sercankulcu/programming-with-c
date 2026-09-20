# Procedural Programming with C — all 56 weeks
# https://sercankulcu.github.io/teaching/programming-with-c/

PORTABLE := week01_computers_programs_and_the_c_toolchain \n            week02_history_standards_and_the_compilation_model \n            week03_program_structure_style_and_basic_output \n            week04_data_representation \n            week05_variables_types_and_constants \n            week06_operators_and_expressions \n            week07_type_conversions \n            week08_formatted_input_and_output \n            week09_conditional_statements \n            week10_loops_and_flow_control \n            week11_functions \n            week12_one_and_multi_dimensional_arrays \n            week13_introduction_to_pointers \n            week14_strings_and_character_handling \n            week15_pointer_arithmetic_and_arrays \n            week16_pointers_as_function_parameters \n            week17_scope_lifetime_and_program_memory_layout \n            week18_recursion \n            week19_dynamic_memory_allocation \n            week20_memory_errors_and_their_diagnosis \n            week21_structures \n            week22_unions_enumerations_and_type_aliases \n            week23_program_arguments_and_input \n            week24_the_standard_library_toolbox \n            week25_bit_level_operations \n            week26_files_and_streams \n            week27_error_handling_strategies \n            week28_the_preprocessor_and_modular_programming \n            week30_function_pointers_callbacks_and_generic_programming \n            week32_linked_data_structures \n            week33_trees_hash_tables_and_abstract_data_types \n            week34_performance_and_memory_layout \n            week35_debugging_testing_and_analysis \n            week36_undefined_behavior_and_secure_coding \n            week37_portability_and_the_machine_model \n            week38_type_qualifiers_inlining_and_modern_c \n            week42_reading_excellent_c \n            week43_working_in_an_existing_codebase \n            week45_packaging_and_distributing_c_libraries \n            week56_maintaining_long_lived_c_codebases

POSIX    := week31_command_line_interfaces_and_the_environment \n            week39_processes_signals_and_low_level_i_o \n            week40_network_programming_with_sockets \n            week41_concurrency_and_the_c_memory_model \n            week47_event_driven_and_scalable_i_o \n            week48_fuzzing_and_security_testing

EMBEDDED := week49_embedded_c_and_cross_compilation \n            week50_memory_mapped_i_o_and_hardware_registers \n            week51_interrupts_and_real_time_behavior \n            week52_linker_scripts_and_firmware_memory_layout \n            week53_constrained_resource_programming \n            week54_real_time_operating_systems \n            week55_embedded_debugging_and_testing

# Week 46 spans C, Python, Rust and C++; it needs more than a C compiler.
POLYGLOT := week46_abi_and_interfacing_with_other_languages

# Weeks whose subject is a build system or a code review, not a program.
NOCODE   := week29_building_programs_and_libraries week44_collaborative_development_and_code_review

all: portable

portable:
	@for d in $(PORTABLE); do echo "==> $$d"; $(MAKE) -s -C $$d || exit 1; done

posix:
	@for d in $(POSIX); do echo "==> $$d"; $(MAKE) -s -C $$d || exit 1; done

embedded:
	@for d in $(EMBEDDED); do echo "==> $$d"; $(MAKE) -s -C $$d || exit 1; done

polyglot:
	@for d in $(POLYGLOT); do echo "==> $$d"; $(MAKE) -s -C $$d || exit 1; done

everything: portable posix embedded polyglot

# make week19  — build one week by number
week%:
	@d=$$(ls -d week$**_* 2>/dev/null | head -1); \
	 if [ -z "$$d" ]; then echo "no such week"; exit 1; fi; \
	 echo "==> $$d"; $(MAKE) -C $$d

clean:
	@for d in week*_*/; do $(MAKE) -s -C $$d clean 2>/dev/null || true; done

.PHONY: all portable posix embedded polyglot everything clean
