// RUN: mlir-opt %s -test-diagnostic-handler
// This test verifies that diagnostic handler can emit the call stack successfully.

func @call_site_loc_in_fused() -> tensor<4x?xf32> {
  %0 = constant dense<3> : tensor<i32>
  // expected-error@+1 {{mysource1: error:}}
  // expected-error@+2 {{mysource2: note: called from}}
  // expected-error@+3 {{mysource3: note: called from}}
  %2 = tensor_cast %0 : tensor<i32> to tensor<4x?xf32>  loc(fused[callsite("foo"("mysource1":0:0) at callsite("mysource2":1:0 at "mysource3":2:0)), "bar"])
  return %2 : tensor<4x?xf32>
}
