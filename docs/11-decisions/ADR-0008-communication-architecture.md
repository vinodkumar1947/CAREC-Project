# ADR-0008 — Communication Architecture

Status: Proposed
Date: 2026-08-23

## Context
ROS 2 nodes, safety MCU, mobile client, and optional cloud have different trust, latency, and availability properties.

## Decision
TBD. Proposed invariant: safety-critical motion remains local; remote links cannot bypass local priority or safety supervision.

## Alternatives Considered
ROS 2/DDS, serial/CAN-like MCU links, BLE/Wi-Fi mobile links, and cloud messaging.

## Advantages
Layered contracts can isolate unreliable or untrusted networks.

## Disadvantages
More gateways, authentication, versioning, and observability work.

## Safety Impact
Timeout, replay, spoofing, partition, stale command, and denial-of-service risks must be controlled.

## Consequences
Specify threat model, schemas, timeouts, authentication, offline behavior, and tests before implementation.
