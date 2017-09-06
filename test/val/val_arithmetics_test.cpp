// Copyright (c) 2017 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Tests for unique type declaration rules validator.

#include <string>

#include "gmock/gmock.h"
#include "unit_spirv.h"
#include "val_fixtures.h"

namespace {

using ::testing::HasSubstr;
using ::testing::Not;

using std::string;

using ValidateArithmetics = spvtest::ValidateBase<bool>;

std::string GenerateCode(const std::string& main_body) {
  const std::string prefix =
R"(
OpCapability Shader
OpCapability Int64
OpCapability Float64
OpCapability Matrix
%ext_inst = OpExtInstImport "GLSL.std.450"
OpMemoryModel Logical GLSL450
OpEntryPoint Fragment %main "main"
%void = OpTypeVoid
%func = OpTypeFunction %void
%bool = OpTypeBool
%f32 = OpTypeFloat 32
%u32 = OpTypeInt 32 0
%s32 = OpTypeInt 32 1
%f64 = OpTypeFloat 64
%u64 = OpTypeInt 64 0
%s64 = OpTypeInt 64 1
%boolvec2 = OpTypeVector %bool 2
%s32vec2 = OpTypeVector %s32 2
%u32vec2 = OpTypeVector %u32 2
%u64vec2 = OpTypeVector %u64 2
%f32vec2 = OpTypeVector %f32 2
%f64vec2 = OpTypeVector %f64 2
%boolvec3 = OpTypeVector %bool 3
%u32vec3 = OpTypeVector %u32 3
%u64vec3 = OpTypeVector %u64 3
%s32vec3 = OpTypeVector %s32 3
%f32vec3 = OpTypeVector %f32 3
%f64vec3 = OpTypeVector %f64 3
%boolvec4 = OpTypeVector %bool 4
%u32vec4 = OpTypeVector %u32 4
%u64vec4 = OpTypeVector %u64 4
%s32vec4 = OpTypeVector %s32 4
%f32vec4 = OpTypeVector %f32 4
%f64vec4 = OpTypeVector %f64 4

%f32mat22 = OpTypeMatrix %f32vec2 2
%f32mat23 = OpTypeMatrix %f32vec2 3
%f32mat32 = OpTypeMatrix %f32vec3 2
%f32mat33 = OpTypeMatrix %f32vec3 3
%f64mat22 = OpTypeMatrix %f64vec2 2

%f32_0 = OpConstant %f32 0
%f32_1 = OpConstant %f32 1
%f32_2 = OpConstant %f32 2
%f32_3 = OpConstant %f32 3
%f32_4 = OpConstant %f32 4
%f32_pi = OpConstant %f32 3.14159

%s32_0 = OpConstant %s32 0
%s32_1 = OpConstant %s32 1
%s32_2 = OpConstant %s32 2
%s32_3 = OpConstant %s32 3
%s32_4 = OpConstant %s32 4
%s32_m1 = OpConstant %s32 -1

%u32_0 = OpConstant %u32 0
%u32_1 = OpConstant %u32 1
%u32_2 = OpConstant %u32 2
%u32_3 = OpConstant %u32 3
%u32_4 = OpConstant %u32 4

%f64_0 = OpConstant %f64 0
%f64_1 = OpConstant %f64 1
%f64_2 = OpConstant %f64 2
%f64_3 = OpConstant %f64 3
%f64_4 = OpConstant %f64 4

%s64_0 = OpConstant %s64 0
%s64_1 = OpConstant %s64 1
%s64_2 = OpConstant %s64 2
%s64_3 = OpConstant %s64 3
%s64_4 = OpConstant %s64 4
%s64_m1 = OpConstant %s64 -1

%u64_0 = OpConstant %u64 0
%u64_1 = OpConstant %u64 1
%u64_2 = OpConstant %u64 2
%u64_3 = OpConstant %u64 3
%u64_4 = OpConstant %u64 4

%u32vec2_01 = OpConstantComposite %u32vec2 %u32_0 %u32_1
%u32vec2_12 = OpConstantComposite %u32vec2 %u32_1 %u32_2
%u32vec3_012 = OpConstantComposite %u32vec3 %u32_0 %u32_1 %u32_2
%u32vec3_123 = OpConstantComposite %u32vec3 %u32_1 %u32_2 %u32_3
%u32vec4_0123 = OpConstantComposite %u32vec4 %u32_0 %u32_1 %u32_2 %u32_3
%u32vec4_1234 = OpConstantComposite %u32vec4 %u32_1 %u32_2 %u32_3 %u32_4

%s32vec2_01 = OpConstantComposite %s32vec2 %s32_0 %s32_1
%s32vec2_12 = OpConstantComposite %s32vec2 %s32_1 %s32_2
%s32vec3_012 = OpConstantComposite %s32vec3 %s32_0 %s32_1 %s32_2
%s32vec3_123 = OpConstantComposite %s32vec3 %s32_1 %s32_2 %s32_3
%s32vec4_0123 = OpConstantComposite %s32vec4 %s32_0 %s32_1 %s32_2 %s32_3
%s32vec4_1234 = OpConstantComposite %s32vec4 %s32_1 %s32_2 %s32_3 %s32_4

%f32vec2_01 = OpConstantComposite %f32vec2 %f32_0 %f32_1
%f32vec2_12 = OpConstantComposite %f32vec2 %f32_1 %f32_2
%f32vec3_012 = OpConstantComposite %f32vec3 %f32_0 %f32_1 %f32_2
%f32vec3_123 = OpConstantComposite %f32vec3 %f32_1 %f32_2 %f32_3
%f32vec4_0123 = OpConstantComposite %f32vec4 %f32_0 %f32_1 %f32_2 %f32_3
%f32vec4_1234 = OpConstantComposite %f32vec4 %f32_1 %f32_2 %f32_3 %f32_4

%f64vec2_01 = OpConstantComposite %f64vec2 %f64_0 %f64_1
%f64vec2_12 = OpConstantComposite %f64vec2 %f64_1 %f64_2
%f64vec3_012 = OpConstantComposite %f64vec3 %f64_0 %f64_1 %f64_2
%f64vec3_123 = OpConstantComposite %f64vec3 %f64_1 %f64_2 %f64_3
%f64vec4_0123 = OpConstantComposite %f64vec4 %f64_0 %f64_1 %f64_2 %f64_3
%f64vec4_1234 = OpConstantComposite %f64vec4 %f64_1 %f64_2 %f64_3 %f64_4

%f32mat22_1212 = OpConstantComposite %f32mat22 %f32vec2_12 %f32vec2_12
%f32mat23_121212 = OpConstantComposite %f32mat23 %f32vec2_12 %f32vec2_12 %f32vec2_12
%f32mat32_123123 = OpConstantComposite %f32mat32 %f32vec3_123 %f32vec3_123
%f32mat33_123123123 = OpConstantComposite %f32mat33 %f32vec3_123 %f32vec3_123 %f32vec3_123

%f64mat22_1212 = OpConstantComposite %f64mat22 %f64vec2_12 %f64vec2_12

%main = OpFunction %void None %func
%main_entry = OpLabel)";

  const std::string suffix =
R"(
OpReturn
OpFunctionEnd)";

  return prefix + main_body + suffix;
}

TEST_F(ValidateArithmetics, F32Success) {
  const std::string body = R"(
%val1 = OpFMul %f32 %f32_0 %f32_1
%val2 = OpFSub %f32 %f32_2 %f32_0
%val3 = OpFAdd %f32 %val1 %val2
%val4 = OpFNegate %f32 %val3
%val5 = OpFDiv %f32 %val4 %val1
%val6 = OpFRem %f32 %val4 %f32_2
%val7 = OpFMod %f32 %val4 %f32_2
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, F64Success) {
  const std::string body = R"(
%val1 = OpFMul %f64 %f64_0 %f64_1
%val2 = OpFSub %f64 %f64_2 %f64_0
%val3 = OpFAdd %f64 %val1 %val2
%val4 = OpFNegate %f64 %val3
%val5 = OpFDiv %f64 %val4 %val1
%val6 = OpFRem %f64 %val4 %f64_2
%val7 = OpFMod %f64 %val4 %f64_2
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, Int32Success) {
  const std::string body = R"(
%val1 = OpIMul %u32 %s32_0 %u32_1
%val2 = OpIMul %s32 %s32_2 %u32_1
%val3 = OpIAdd %u32 %val1 %val2
%val4 = OpIAdd %s32 %val1 %val2
%val5 = OpISub %u32 %val3 %val4
%val6 = OpISub %s32 %val4 %val3
%val7 = OpSDiv %s32 %val4 %val3
%val8 = OpSNegate %s32 %val7
%val9 = OpSRem %s32 %val4 %val3
%val10 = OpSMod %s32 %val4 %val3
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, Int64Success) {
  const std::string body = R"(
%val1 = OpIMul %u64 %s64_0 %u64_1
%val2 = OpIMul %s64 %s64_2 %u64_1
%val3 = OpIAdd %u64 %val1 %val2
%val4 = OpIAdd %s64 %val1 %val2
%val5 = OpISub %u64 %val3 %val4
%val6 = OpISub %s64 %val4 %val3
%val7 = OpSDiv %s64 %val4 %val3
%val8 = OpSNegate %s64 %val7
%val9 = OpSRem %s64 %val4 %val3
%val10 = OpSMod %s64 %val4 %val3
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, F32Vec2Success) {
  const std::string body = R"(
%val1 = OpFMul %f32vec2 %f32vec2_01 %f32vec2_12
%val2 = OpFSub %f32vec2 %f32vec2_12 %f32vec2_01
%val3 = OpFAdd %f32vec2 %val1 %val2
%val4 = OpFNegate %f32vec2 %val3
%val5 = OpFDiv %f32vec2 %val4 %val1
%val6 = OpFRem %f32vec2 %val4 %f32vec2_12
%val7 = OpFMod %f32vec2 %val4 %f32vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, F64Vec2Success) {
  const std::string body = R"(
%val1 = OpFMul %f64vec2 %f64vec2_01 %f64vec2_12
%val2 = OpFSub %f64vec2 %f64vec2_12 %f64vec2_01
%val3 = OpFAdd %f64vec2 %val1 %val2
%val4 = OpFNegate %f64vec2 %val3
%val5 = OpFDiv %f64vec2 %val4 %val1
%val6 = OpFRem %f64vec2 %val4 %f64vec2_12
%val7 = OpFMod %f64vec2 %val4 %f64vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, U32Vec2Success) {
  const std::string body = R"(
%val1 = OpIMul %u32vec2 %u32vec2_01 %u32vec2_12
%val2 = OpISub %u32vec2 %u32vec2_12 %u32vec2_01
%val3 = OpIAdd %u32vec2 %val1 %val2
%val4 = OpSNegate %u32vec2 %val3
%val5 = OpSDiv %u32vec2 %val4 %val1
%val6 = OpSRem %u32vec2 %val4 %u32vec2_12
%val7 = OpSMod %u32vec2 %val4 %u32vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, FNegateTypeIdU32) {
  const std::string body = R"(
%val = OpFNegate %u32 %u32_0
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected floating scalar or vector type as type_id: FNegate"));
}

TEST_F(ValidateArithmetics, FNegateTypeIdVec2U32) {
  const std::string body = R"(
%val = OpFNegate %u32vec2 %u32vec2_01
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected floating scalar or vector type as type_id: FNegate"));
}

TEST_F(ValidateArithmetics, FNegateWrongOperand) {
  const std::string body = R"(
%val = OpFNegate %f32 %u32_0
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have type type_id: "
      "FNegate operand index 2"));
}

TEST_F(ValidateArithmetics, FMulTypeIdU32) {
  const std::string body = R"(
%val = OpFMul %u32 %u32_0 %u32_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected floating scalar or vector type as type_id: FMul"));
}

TEST_F(ValidateArithmetics, FMulTypeIdVec2U32) {
  const std::string body = R"(
%val = OpFMul %u32vec2 %u32vec2_01 %u32vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected floating scalar or vector type as type_id: FMul"));
}

TEST_F(ValidateArithmetics, FMulWrongOperand1) {
  const std::string body = R"(
%val = OpFMul %f32 %u32_0 %f32_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have type type_id: "
      "FMul operand index 2"));
}

TEST_F(ValidateArithmetics, FMulWrongOperand2) {
  const std::string body = R"(
%val = OpFMul %f32 %f32_0 %u32_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have type type_id: "
      "FMul operand index 3"));
}

TEST_F(ValidateArithmetics, FMulWrongVectorOperand1) {
  const std::string body = R"(
%val = OpFMul %f64vec3 %f32vec3_123 %f64vec3_012
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have type type_id: "
      "FMul operand index 2"));
}

TEST_F(ValidateArithmetics, FMulWrongVectorOperand2) {
  const std::string body = R"(
%val = OpFMul %f32vec3 %f32vec3_123 %f64vec3_012
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have type type_id: "
      "FMul operand index 3"));
}

TEST_F(ValidateArithmetics, IMulFloatTypeId) {
  const std::string body = R"(
%val = OpIMul %f32 %u32_0 %s32_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected int scalar or vector type as type_id: IMul"));
}

TEST_F(ValidateArithmetics, IMulFloatOperand1) {
  const std::string body = R"(
%val = OpIMul %u32 %f32_0 %s32_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected int scalar or vector type as operand: "
      "IMul operand index 2"));
}

TEST_F(ValidateArithmetics, IMulFloatOperand2) {
  const std::string body = R"(
%val = OpIMul %u32 %s32_0 %f32_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected int scalar or vector type as operand: "
      "IMul operand index 3"));
}

TEST_F(ValidateArithmetics, IMulWrongBitWidthOperand1) {
  const std::string body = R"(
%val = OpIMul %u64 %u32_0 %s64_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have the same bit width "
      "as type_id: IMul operand index 2"));
}

TEST_F(ValidateArithmetics, IMulWrongBitWidthOperand2) {
  const std::string body = R"(
%val = OpIMul %u32 %u32_0 %s64_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have the same bit width "
      "as type_id: IMul operand index 3"));
}

TEST_F(ValidateArithmetics, IMulWrongBitWidthVector) {
  const std::string body = R"(
%val = OpIMul %u64vec3 %u32vec3_012 %u32vec3_123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have the same bit width "
      "as type_id: IMul operand index 2"));
}

TEST_F(ValidateArithmetics, IMulVectorScalarOperand1) {
  const std::string body = R"(
%val = OpIMul %u32vec2 %u32_0 %u32vec2_01
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have the same dimension "
      "as type_id: IMul operand index 2"));
}

TEST_F(ValidateArithmetics, IMulVectorScalarOperand2) {
  const std::string body = R"(
%val = OpIMul %u32vec2 %u32vec2_01 %u32_0
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have the same dimension "
      "as type_id: IMul operand index 3"));
}

TEST_F(ValidateArithmetics, IMulScalarVectorOperand1) {
  const std::string body = R"(
%val = OpIMul %s32 %u32vec2_01 %u32_0
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have the same dimension "
      "as type_id: IMul operand index 2"));
}

TEST_F(ValidateArithmetics, IMulScalarVectorOperand2) {
  const std::string body = R"(
%val = OpIMul %u32 %u32_0 %s32vec2_01
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have the same dimension "
      "as type_id: IMul operand index 3"));
}

TEST_F(ValidateArithmetics, SNegateFloat) {
  const std::string body = R"(
%val = OpSNegate %s32 %f32_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected int scalar or vector type as operand: "
      "SNegate operand index 2"));
}

TEST_F(ValidateArithmetics, UDivFloatType) {
  const std::string body = R"(
%val = OpUDiv %f32 %u32_2 %u32_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected unsigned int scalar or vector type as type_id: UDiv"));
}

TEST_F(ValidateArithmetics, UDivSignedIntType) {
  const std::string body = R"(
%val = OpUDiv %s32 %u32_2 %u32_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected unsigned int scalar or vector type as type_id: UDiv"));
}

TEST_F(ValidateArithmetics, UDivWrongOperand1) {
  const std::string body = R"(
%val = OpUDiv %u64 %f64_2 %u64_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have type type_id: "
      "UDiv operand index 2"));
}

TEST_F(ValidateArithmetics, UDivWrongOperand2) {
  const std::string body = R"(
%val = OpUDiv %u64 %u64_2 %u32_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected arithmetic operands to have type type_id: "
      "UDiv operand index 3"));
}

TEST_F(ValidateArithmetics, DotSuccess) {
  const std::string body = R"(
%val = OpDot %f32 %f32vec2_01 %f32vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, DotWrongTypeId) {
  const std::string body = R"(
%val = OpDot %u32 %u32vec2_01 %u32vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float scalar type as type_id: Dot"));
}

TEST_F(ValidateArithmetics, DotNotVectorTypeOperand1) {
  const std::string body = R"(
%val = OpDot %f32 %f32 %f32vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float vector as operand: Dot operand index 2"));
}

TEST_F(ValidateArithmetics, DotNotVectorTypeOperand2) {
  const std::string body = R"(
%val = OpDot %f32 %f32vec3_012 %f32_1
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float vector as operand: Dot operand index 3"));
}

TEST_F(ValidateArithmetics, DotWrongComponentOperand1) {
  const std::string body = R"(
%val = OpDot %f64 %f32vec2_01 %f64vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected component type to be equal to type_id: Dot operand index 2"));
}

TEST_F(ValidateArithmetics, DotWrongComponentOperand2) {
  const std::string body = R"(
%val = OpDot %f32 %f32vec2_01 %f64vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected component type to be equal to type_id: Dot operand index 3"));
}

TEST_F(ValidateArithmetics, DotDifferentVectorSize) {
  const std::string body = R"(
%val = OpDot %f32 %f32vec2_01 %f32vec3_123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected operands to have the same number of componenets: Dot"));
}

TEST_F(ValidateArithmetics, VectorTimesScalarSuccess) {
  const std::string body = R"(
%val = OpVectorTimesScalar %f32vec2 %f32vec2_01 %f32_2
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, VectorTimesScalarWrongTypeId) {
  const std::string body = R"(
%val = OpVectorTimesScalar %u32vec2 %f32vec2_01 %f32_2
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float vector type as type_id: "
      "VectorTimesScalar"));
}

TEST_F(ValidateArithmetics, VectorTimesScalarWrongVector) {
  const std::string body = R"(
%val = OpVectorTimesScalar %f32vec2 %f32vec3_012 %f32_2
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected vector operand type to be equal to type_id: "
      "VectorTimesScalar"));
}

TEST_F(ValidateArithmetics, VectorTimesScalarWrongScalar) {
  const std::string body = R"(
%val = OpVectorTimesScalar %f32vec2 %f32vec2_01 %f64_2
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected scalar operand type to be equal to the component "
      "type of the vector operand: VectorTimesScalar"));
}

TEST_F(ValidateArithmetics, MatrixTimesScalarSuccess) {
  const std::string body = R"(
%val = OpMatrixTimesScalar %f32mat22 %f32mat22_1212 %f32_2
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, MatrixTimesScalarWrongTypeId) {
  const std::string body = R"(
%val = OpMatrixTimesScalar %f32vec2 %f32mat22_1212 %f32_2
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float matrix type as type_id: "
      "MatrixTimesScalar"));
}

TEST_F(ValidateArithmetics, MatrixTimesScalarWrongMatrix) {
  const std::string body = R"(
%val = OpMatrixTimesScalar %f32mat22 %f32vec2_01 %f32_2
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected matrix operand type to be equal to type_id: "
      "MatrixTimesScalar"));
}

TEST_F(ValidateArithmetics, MatrixTimesScalarWrongScalar) {
  const std::string body = R"(
%val = OpMatrixTimesScalar %f32mat22 %f32mat22_1212 %f64_2
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected scalar operand type to be equal to the component "
      "type of the matrix operand: MatrixTimesScalar"));
}

TEST_F(ValidateArithmetics, VectorTimesMatrix2x22Success) {
  const std::string body = R"(
%val = OpVectorTimesMatrix %f32vec2 %f32vec2_12 %f32mat22_1212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, VectorTimesMatrix3x32Success) {
  const std::string body = R"(
%val = OpVectorTimesMatrix %f32vec2 %f32vec3_123 %f32mat32_123123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, VectorTimesMatrixWrongTypeId) {
  const std::string body = R"(
%val = OpVectorTimesMatrix %f32mat22 %f32vec2_12 %f32mat22_1212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float vector type as type_id: "
      "VectorTimesMatrix"));
}

TEST_F(ValidateArithmetics, VectorTimesMatrixNotFloatVector) {
  const std::string body = R"(
%val = OpVectorTimesMatrix %f32vec2 %u32vec2_12 %f32mat22_1212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float vector type as left operand: "
      "VectorTimesMatrix"));
}

TEST_F(ValidateArithmetics, VectorTimesMatrixWrongVectorComponent) {
  const std::string body = R"(
%val = OpVectorTimesMatrix %f32vec2 %f64vec2_12 %f32mat22_1212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected component types of type_id and vector to be equal: "
      "VectorTimesMatrix"));
}

TEST_F(ValidateArithmetics, VectorTimesMatrixWrongMatrix) {
  const std::string body = R"(
%val = OpVectorTimesMatrix %f32vec2 %f32vec2_12 %f32vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float matrix type as right operand: "
      "VectorTimesMatrix"));
}

TEST_F(ValidateArithmetics, VectorTimesMatrixWrongMatrixComponent) {
  const std::string body = R"(
%val = OpVectorTimesMatrix %f32vec2 %f32vec2_12 %f64mat22_1212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected component types of type_id and matrix to be equal: "
      "VectorTimesMatrix"));
}

TEST_F(ValidateArithmetics, VectorTimesMatrix2eq2x23Fail) {
  const std::string body = R"(
%val = OpVectorTimesMatrix %f32vec2 %f32vec2_12 %f32mat23_121212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected number of columns of the matrix to be equal to the type_id "
      "vector size: VectorTimesMatrix"));
}

TEST_F(ValidateArithmetics, VectorTimesMatrix2x32Fail) {
  const std::string body = R"(
%val = OpVectorTimesMatrix %f32vec2 %f32vec2_12 %f32mat32_123123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected number of rows of the matrix to be equal to the vector "
      "operand size: VectorTimesMatrix"));
}

TEST_F(ValidateArithmetics, MatrixTimesVector22x2Success) {
  const std::string body = R"(
%val = OpMatrixTimesVector %f32vec2 %f32mat22_1212 %f32vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, MatrixTimesVector23x3Success) {
  const std::string body = R"(
%val = OpMatrixTimesVector %f32vec2 %f32mat23_121212 %f32vec3_123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, MatrixTimesVectorWrongTypeId) {
  const std::string body = R"(
%val = OpMatrixTimesVector %f32mat22 %f32mat22_1212 %f32vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float vector type as type_id: "
      "MatrixTimesVector"));
}

TEST_F(ValidateArithmetics, MatrixTimesVectorWrongMatrix) {
  const std::string body = R"(
%val = OpMatrixTimesVector %f32vec3 %f32vec3_123 %f32vec3_123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float matrix type as left operand: "
      "MatrixTimesVector"));
}

TEST_F(ValidateArithmetics, MatrixTimesVectorWrongMatrixCol) {
  const std::string body = R"(
%val = OpMatrixTimesVector %f32vec3 %f32mat23_121212 %f32vec3_123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected column type of the matrix to be equal to type_id: "
      "MatrixTimesVector"));
}

TEST_F(ValidateArithmetics, MatrixTimesVectorWrongVector) {
  const std::string body = R"(
%val = OpMatrixTimesVector %f32vec2 %f32mat22_1212 %u32vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float vector type as right operand: "
      "MatrixTimesVector"));
}

TEST_F(ValidateArithmetics, MatrixTimesVectorDifferentComponents) {
  const std::string body = R"(
%val = OpMatrixTimesVector %f32vec2 %f32mat22_1212 %f64vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected component types of the operands to be equal: "
      "MatrixTimesVector"));
}

TEST_F(ValidateArithmetics, MatrixTimesVector22x3Fail) {
  const std::string body = R"(
%val = OpMatrixTimesVector %f32vec2 %f32mat22_1212 %f32vec3_123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected number of columns of the matrix to be equal to the vector "
      "size: MatrixTimesVector"));
}

TEST_F(ValidateArithmetics, MatrixTimesMatrix22x22Success) {
  const std::string body = R"(
%val = OpMatrixTimesMatrix %f32mat22 %f32mat22_1212 %f32mat22_1212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, MatrixTimesMatrix23x32Success) {
  const std::string body = R"(
%val = OpMatrixTimesMatrix %f32mat22 %f32mat23_121212 %f32mat32_123123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, MatrixTimesMatrix33x33Success) {
  const std::string body = R"(
%val = OpMatrixTimesMatrix %f32mat33 %f32mat33_123123123 %f32mat33_123123123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, MatrixTimesMatrixWrongTypeId) {
  const std::string body = R"(
%val = OpMatrixTimesMatrix %f32vec2 %f32mat22_1212 %f32mat22_1212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float matrix type as type_id: MatrixTimesMatrix"));
}

TEST_F(ValidateArithmetics, MatrixTimesMatrixWrongLeftOperand) {
  const std::string body = R"(
%val = OpMatrixTimesMatrix %f32mat22 %f32vec2_12 %f32mat22_1212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float matrix type as left operand: MatrixTimesMatrix"));
}

TEST_F(ValidateArithmetics, MatrixTimesMatrixWrongRightOperand) {
  const std::string body = R"(
%val = OpMatrixTimesMatrix %f32mat22 %f32mat22_1212 %f32vec2_12
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float matrix type as right operand: MatrixTimesMatrix"));
}

TEST_F(ValidateArithmetics, MatrixTimesMatrix32x23Fail) {
  const std::string body = R"(
%val = OpMatrixTimesMatrix %f32mat22 %f32mat32_123123 %f32mat23_121212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected column types of type_id and left matrix to be equal: "
      "MatrixTimesMatrix"));
}

TEST_F(ValidateArithmetics, MatrixTimesMatrixDifferentComponents) {
  const std::string body = R"(
%val = OpMatrixTimesMatrix %f32mat22 %f32mat22_1212 %f64mat22_1212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected component types of type_id and right matrix to be equal: "
      "MatrixTimesMatrix"));
}

TEST_F(ValidateArithmetics, MatrixTimesMatrix23x23Fail) {
  const std::string body = R"(
%val = OpMatrixTimesMatrix %f32mat22 %f32mat23_121212 %f32mat23_121212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected number of columns of type_id and right matrix to be equal: "
      "MatrixTimesMatrix"));
}

TEST_F(ValidateArithmetics, MatrixTimesMatrix23x22Fail) {
  const std::string body = R"(
%val = OpMatrixTimesMatrix %f32mat22 %f32mat23_121212 %f32mat22_1212
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected number of columns of left matrix and number of rows of right "
      "matrix to be equal: MatrixTimesMatrix"));
}

TEST_F(ValidateArithmetics, OuterProduct2x2Success) {
  const std::string body = R"(
%val = OpOuterProduct %f32mat22 %f32vec2_12 %f32vec2_01
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, OuterProduct3x2Success) {
  const std::string body = R"(
%val = OpOuterProduct %f32mat32 %f32vec3_123 %f32vec2_01
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, OuterProduct2x3Success) {
  const std::string body = R"(
%val = OpOuterProduct %f32mat23 %f32vec2_01 %f32vec3_123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_SUCCESS, ValidateInstructions());
}

TEST_F(ValidateArithmetics, OuterProductWrongTypeId) {
  const std::string body = R"(
%val = OpOuterProduct %f32vec2 %f32vec2_01 %f32vec3_123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float matrix type as type_id: "
      "OuterProduct"));
}

TEST_F(ValidateArithmetics, OuterProductWrongLeftOperand) {
  const std::string body = R"(
%val = OpOuterProduct %f32mat22 %f32vec3_123 %f32vec2_01
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected column type of the type_id to be equal to the type "
      "of the left operand: OuterProduct"));
}

TEST_F(ValidateArithmetics, OuterProductRightOperandNotFloatVector) {
  const std::string body = R"(
%val = OpOuterProduct %f32mat22 %f32vec2_12 %u32vec2_01
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected float vector type as right operand: OuterProduct"));
}

TEST_F(ValidateArithmetics, OuterProductRightOperandWrongComponent) {
  const std::string body = R"(
%val = OpOuterProduct %f32mat22 %f32vec2_12 %f64vec2_01
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected component types of the operands to be equal: OuterProduct"));
}

TEST_F(ValidateArithmetics, OuterProductRightOperandWrongDimension) {
  const std::string body = R"(
%val = OpOuterProduct %f32mat22 %f32vec2_12 %f32vec3_123
)";

  CompileSuccessfully(GenerateCode(body).c_str());
  ASSERT_EQ(SPV_ERROR_INVALID_DATA, ValidateInstructions());
  EXPECT_THAT(getDiagnosticString(), HasSubstr(
      "Expected number of columns of the matrix to be equal to the "
      "vector size of the right operand: OuterProduct"));
}

}  // anonymous namespace
