const {test} = require('node:test');
const assert = require('node:assert/strict');
const api = require('../../pilot/core.js');
test('a local video cannot inherit sample observations', () => {
  const sample = api.review(api.sample(), 'sample-1', 'confirmed');
  assert.equal(sample.findings[0].status, 'confirmed');
  assert.deepEqual(api.localVideo('private.mp4').findings, []);
});
test('reports preserve origin, review decisions, and the absence of automatic analysis', () => {
  const state = api.review(api.sample(), 'sample-2', 'dismissed');
  const report = api.report(state, '2026-11-17T00:00:00Z');
  assert.equal(report.automatic_inference, false);
  assert.equal(report.source_kind, 'synthetic_sample');
  assert.equal(report.findings[1].status, 'dismissed');
  assert.equal(report.findings[1].origin, 'authored_fixture');
  report.findings[0].status = 'confirmed';
  assert.equal(state.findings[0].status, 'unreviewed');
});
test('invalid annotations and status changes are rejected', () => {
  for (const timestamp of [-1, NaN, Infinity]) assert.throws(() => api.finding('a', timestamp, 'Barrier'));
  for (const note of ['', ' ', 'x'.repeat(501)]) assert.throws(() => api.finding('a', 0, note));
  assert.throws(() => api.review(api.sample(), 'sample-1', 'certified_safe'));
  assert.throws(() => api.review(api.sample(), 'missing', 'confirmed'));
});
test('manual annotations stay distinguishable from authored examples', () => {
  const state = api.localVideo('example.webm');
  state.findings.push(api.finding('manual-1', 1.5, '  Check this passage  '));
  const report = api.report(state);
  assert.equal(report.findings[0].origin, 'manual');
  assert.equal(report.findings[0].description, 'Check this passage');
  assert.equal(report.findings[0].timestamp_s, 1.5);
});
