# Procedural Programming with C - all 56 weeks
# https://sercankulcu.github.io/teaching/procedural-programming-with-c

PORTABLE := week01_computers_programs_and_the_c_toolchain week02_history_standards_and_the_compilation_model week03_program_structure_style_and_basic_output week04_data_representation week05_variables_types_and_constants week06_operators_and_expressions week07_type_conversions week08_formatted_input_and_output week09_conditional_statements week10_loops_and_flow_control week11_functions week12_one_and_multi_dimensional_arrays week13_introduction_to_pointers week14_strings_and_character_handling week15_pointer_arithmetic_and_arrays week16_pointers_as_function_parameters week17_scope_lifetime_and_program_memory_layout week18_recursion week19_dynamic_memory_allocation week20_memory_errors_and_their_diagnosis week21_structures week22_unions_enumerations_and_type_aliases week23_program_arguments_and_input week24_the_standard_library_toolbox week25_bit_level_operations week26_files_and_streams week27_error_handling_strategies week28_the_preprocessor_and_modular_programming week30_function_pointers_callbacks_and_generic_programming week32_linked_data_structures week33_trees_hash_tables_and_abstract_data_types week34_performance_and_memory_layout week35_debugging_testing_and_analysis week36_undefined_behavior_and_secure_coding week37_portability_and_the_machine_model week38_type_qualifiers_inlining_and_modern_c week42_reading_excellent_c week43_working_in_an_existing_codebase week45_packaging_and_distributing_c_libraries week56_maintaining_long_lived_c_codebases week55_embedded_debugging_and_testing

POSIX := week31_command_line_interfaces_and_the_environment week39_processes_signals_and_low_level_i_o week40_network_programming_with_sockets week41_concurrency_and_the_c_memory_model week47_event_driven_and_scalable_i_o week48_fuzzing_and_security_testing

EMBEDDED := week49_embedded_c_and_cross_compilation week50_memory_mapped_i_o_and_hardware_registers week51_interrupts_and_real_time_behavior week52_linker_scripts_and_firmware_memory_layout week53_constrained_resource_programming week54_real_time_operating_systems

POLYGLOT := week46_abi_and_interfacing_with_other_languages

# Weeks whose subject is a build system or a code review, not a program.
NOCODE := week29_building_programs_and_libraries week44_collaborative_development_and_code_review

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

# make week19  - build a single week by number
week%:
	@d=`ls -d week$*_* 2>/dev/null | head -1`; test -n "$$d" || { echo "no week $*"; exit 1; }; echo "==> $$d"; $(MAKE) -C $$d

clean:
	@for d in week*_*/; do $(MAKE) -s -C $$d clean >/dev/null 2>&1 || true; done

# used by ci/build.sh to enumerate a group
print-%:
	@echo $($*)

.PHONY: all portable posix embedded polyglot everything clean
