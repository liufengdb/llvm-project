//===- TestDiagnosticHandler.cpp - Pass to test diagnostic handler --------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "mlir/Dialect/StandardOps/Ops.h"
#include "mlir/Pass/Pass.h"

using namespace mlir;

namespace {
struct TestSourceMgrDiagnosticHandler : public SourceMgrDiagnosticHandler {
  explicit TestSourceMgrDiagnosticHandler(llvm::SourceMgr &mgr,
                                          MLIRContext *ctx)
      : SourceMgrDiagnosticHandler(mgr, ctx, llvm::outs()) {}

  void emitTestDiagnostic(Operation *op) {
    Diagnostic diag(op->getLoc(), DiagnosticSeverity::Note);
    emitDiagnostic(diag);
  }
};

/// Pass that emit diagnostic for each op.
struct TestDiagnosticHandler : public FunctionPass<TestDiagnosticHandler> {

  void runOnFunction() override {
    llvm::SourceMgr fileSourceMgr;
    TestSourceMgrDiagnosticHandler diagHandler(fileSourceMgr, &getContext());

    getFunction().walk([&](Operation *op) {
      if (isa<FuncOp>(op) || op->isKnownTerminator())
        return;
      diagHandler.emitTestDiagnostic(op);
    });
  }
};

} // end anonymous namespace

static PassRegistration<TestDiagnosticHandler> pass("test-diagnostic-handler",
                                                    "emit diagnostic for ops");
