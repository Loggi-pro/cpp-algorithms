#include "examples/editor.h"
#include <gtest/gtest.h>
TEST(TextEditorTest, InsertAndPrintAll) {
	TextEditor editor;

	editor.parse("$Insert");
	editor.parse("First line");
	editor.parse("Second line");
	editor.parse("Third line");
	editor.parse("Fourth line");
	editor.parse("Fifth line");
	editor.parse("Sixth line");
	// editor.parse("Line 2");
	auto op = editor.parse("$Done");

	EXPECT_EQ(true, op.has_value());
	EXPECT_EQ(6, op.value()->size());
	auto it = (*op)->begin();
	EXPECT_EQ("First line", *it++);
	EXPECT_EQ("Second line", *it++);
	EXPECT_EQ("Third line", *it++);
	EXPECT_EQ("Fourth line", *it++);
	EXPECT_EQ("Fifth line", *it++);
	EXPECT_EQ("Sixth line", *it++);
}

TEST(TextEditorTest, PrintPart) {
	TextEditor editor;

	editor.parse("$Insert");
	editor.parse("First line");
	editor.parse("Second line");
	editor.parse("Third line");
	editor.parse("Fourth line");
	editor.parse("Fifth line");
	editor.parse("Sixth line");
	// editor.parse("Line 2");
	auto op = editor.parse("$Print 2 3");

	EXPECT_EQ(true, op.has_value());
	EXPECT_EQ(2, op.value()->size());
	auto it = (*op)->begin();
	EXPECT_EQ("Second line", *it++);
	EXPECT_EQ("Third line", *it++);
}