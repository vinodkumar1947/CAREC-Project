# ADR-0007 — AI Inference Platform

Status: Proposed
Date: 2026-08-23

## Context
Perception research needs reproducible inference across contributor and eventual onboard environments.

## Decision
TBD. Begin with framework-neutral model/evaluation contracts; PyTorch and OpenCV are candidates.

## Alternatives Considered
PyTorch, ONNX Runtime, TensorRT, OpenVINO, vendor NPUs, and non-AI baselines.

## Advantages
Portable contracts delay hardware lock-in and support CPU-first contribution.

## Disadvantages
Lowest-common-denominator support may limit performance.

## Safety Impact
Latency, quantization, domain shift, confidence, and fallback require validation.

## Consequences
Require model cards, versioning, benchmark data, and deterministic safety boundaries.
