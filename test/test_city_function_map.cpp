/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <gtest/gtest.h>
#include <engine/engine.h>
#include <engine/log.h>
#include <city_function_map.h>

class FunctionMapTest : public ::testing::Test, public neko::BasicEngine
{
protected:
	void SetUp() override {}

	neko::FunctionMap funcMap_ = 
		neko::FunctionMap(std::make_shared<neko::Component>(NULL));
};

TEST_F(FunctionMapTest, FunctionDoesntRespondInCaseDoesntExist)
{
	EXPECT_FALSE(funcMap_.CallFunction("doesntExist", 0.0));
}

TEST_F(FunctionMapTest, FunctionRespondInCaseExist)
{
	// Register a new function (under exist).
	funcMap_.SetFunction(
		"exist", 
		[](std::shared_ptr<neko::Component> comp, double value) ->bool 
	{
		return true;
	});
	// Try to execute it!
	EXPECT_TRUE(funcMap_.CallFunction("exist", 0.0));
}