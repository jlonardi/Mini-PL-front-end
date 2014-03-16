#pragma once

#ifndef SYMBOL_H
#define SYMBOL_H

enum class Symbol {	error, identifier, integer, op_div, whitespace, op_mult, 
				op_plus, op_minus, op_less, colon, op_equal, op_and, 
				op_not, semicolon, lparen, rparen, string, op_assign, 
				range, comment, var, for_stmt, end, in, do_stmt, read_stmt, 
				print, boolean, assert, end_of_text, newline, string_literal};

#endif
