/*
 * Single-header style dynamic stack.
 * 
 * Copyright (C) 2026 Rayleigh Gamma
 * See LICENSE for licensing information.
 */

#ifndef RAYUTILS_STACK_H
#define RAYUTILS_STACK_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "rayutils/diagnostics/log.h"

#define RAY_STACK_DECLARATION(type, type_name, function_name)								\
	typedef struct { 																		\
		type *elements; 																	\
		size_t count; 																		\
		void (*free_function)(type *const element); 										\
	} type_name##Stack; 																	\
																							\
	bool function_name##_stack_init(type_name##Stack *const stack); 						\
	bool function_name##_stack_push(type_name##Stack *const stack, type const element);		\
	void function_name##_stack_free(type_name##Stack *const stack);

#define RAY_STACK_IMPLEMENTATION(type, type_name, function_name, _free_function) 			\
	bool function_name##_stack_init(type_name##Stack *const stack) { 						\
		stack->elements = malloc(sizeof(type)); 											\
		stack->count = 0;																	\
																							\
		if (stack->elements == NULL) { 														\
			RAY_LOG_ERRNO(errno, "malloc(%zu)", sizeof(type)); 								\
			return false; 																	\
		} 																					\
																							\
		stack->free_function = _free_function;												\
		return true; 																		\
	} 																						\
																							\
	bool function_name##_stack_push(type_name##Stack *const stack, type const element) { 	\
		assert(stack != NULL); 																\
																							\
		const size_t elements_new_size = (stack->count + 1) * sizeof(type); 				\
		type *elements_new = realloc(stack->elements, elements_new_size); 					\
																							\
		if (elements_new == NULL) { 														\
			RAY_LOG_ERRNO(errno, "realloc(%p, %zu)", stack->elements, elements_new_size); 	\
			return false; 																	\
		} 																					\
																							\
		stack->elements = elements_new; 													\
		stack->elements[stack->count++] = element; 											\
		return true; 																		\
	}

#define RAY_STACK_FREE_IMPLEMENTATION(type, type_name, function_name) 	\
	void function_name##_stack_free(type_name##Stack *const stack) { 	\
		assert(stack != NULL); 											\
		assert(stack->elements != NULL); 								\
																		\
		if (stack->free_function != NULL)								\
		{																\
			for (size_t index = 0; index < stack->count; ++index)		\
			{															\
				stack->free_function(&stack->elements[index]);			\
			}															\
		}																\
																		\
		free(stack->elements);											\
	}

#define RAY_STACK_POINTER_FREE_IMPLEMENTATION(type, type_name, function_name)	\
	void function_name##_stack_free(type_name##Stack *const stack) { 			\
		assert(stack != NULL); 													\
		assert(stack->elements != NULL); 										\
																				\
		if (stack->free_function != NULL)										\
		{																		\
			for (size_t index = 0; index < stack->count; ++index)				\
			{																	\
				stack->free_function(stack->elements[index]);					\
				free(stack->elements[index]);									\
			}																	\
		}																		\
																				\
		free(stack->elements);													\
	}

#endif /* RAYUTILS_STACK_H */
