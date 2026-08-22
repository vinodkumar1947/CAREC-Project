# CAREC Project — Documentation Index

**CAREC** (Collision Avoidance with Real-time Edge Computing)  
AI-powered obstacle detection for a 6-year-old's electric wheelchair. Built on **SenseCAP Watcher W1-A Clear Enclosure** — "The Physical AI Agent for Smarter Spaces" (ESP32-S3 + Himax WiseEye2 HX6538 NPU + 5MP OV5647 camera + 1.45" touch screen + 1W speaker).

---

## Hardware Status

- **Device:** SenseCAP Watcher W1-A Clear Enclosure (SKU 113991315, $59.99) — ordered April 28, 2026
- **Phase:** Hardware arriving → Development starts Week 1, May 2026
- **See:** [HARDWARE_STATUS_UPDATE.md](specifications/HARDWARE_STATUS_UPDATE.md) for full timeline

---

## Core Documentation

| Document | Description |
|----------|-------------|
| [Hardware Status Update](specifications/HARDWARE_STATUS_UPDATE.md) | Hardware decision, timeline, what was ordered |
| [SenseCAP Watcher Selection](specifications/SENSECAP_WATCHER_HARDWARE_SELECTION.md) | Full specs, competitor analysis, rationale |
| [System Specification](specifications/system_spec.md) | Full architecture, safety requirements, 90-day roadmap |
| [Quick Reference](guides/quick_reference.md) | Project at a glance, action checklist, firmware starter for SenseCAP Watcher |

## Business & Market

| Document | Description |
|----------|-------------|
| [Competitive Analysis](business/competitive_analysis.md) | Braze Mobility vs CAREC, SWOT, market gap |

## Learning Resources

| Document | Description |
|----------|-------------|
| [DIY Projects & GitHub Links](resources/diy_projects_github.md) | 12 reference implementations, learning path |

## Hardware

| Document | Description |
|----------|-------------|
| [Hardware README](../hardware/README.md) | SenseCAP Watcher mounting guide, accessories |
| [Bill of Materials](../hardware/BOM.md) | Full BOM with costs and suppliers |

## Firmware & Programming

| Document | Description |
|----------|-------------|
| **[Programming Guide](guides/programming_guide.md)** | **Compile, configure & flash — start here** |
| [Firmware API Reference](api/firmware_api.md) | Public API for all 7 firmware modules |
| [Firmware README](../firmware/README.md) | Architecture overview, safety logic |
| [Firmware Setup Guide](../firmware/SETUP.md) | IDE installation + board package detail |
| [Library Requirements](../firmware/main/idf_component.yml) | Required libraries + install steps |

## Project Management

| Document | Description |
|----------|-------------|
| [Development Tasks CSV](../docs/CAREC_tasks.csv) | 49 tasks (SenseCAP Watcher W1-A), ClickUp-ready import |
| [Weekly Review Template](../weekly-reviews/TEMPLATE.md) | Fill weekly: metrics, wins, blockers |

---

## Status

- **Phase:** Hardware ordered → Arriving May 2026
- **Target:** Working obstacle detection in 4 weeks after hardware arrives
- **Budget:** ~$93–112 total (SenseCAP Watcher W1-A Clear $59.99 + mount + battery)
- **Competitor:** Braze Mobility ($4,000) — CAREC is 15–30x cheaper with more features
