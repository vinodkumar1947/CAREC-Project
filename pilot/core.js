/* CAREC accessibility-review contract. No inference or physical control. */
(function (root) {
  'use strict';
  const statuses = ['unreviewed', 'confirmed', 'dismissed'];
  function finding(id, timestamp_s, description, origin = 'manual') {
    if (!Number.isFinite(timestamp_s) || timestamp_s < 0) throw new Error('Timestamp must be a non-negative number.');
    if (typeof description !== 'string' || !description.trim() || description.length > 500) throw new Error('Enter an observation of 1–500 characters.');
    if (!['manual', 'authored_fixture'].includes(origin)) throw new Error('Unknown observation origin.');
    return {id, timestamp_s, description: description.trim(), origin, status: 'unreviewed'};
  }
  function sample() {
    return {source_kind: 'synthetic_sample', source_name: 'Illustrated hallway walkthrough', findings: [
      finding('sample-1', 3, 'A chair appears to obstruct the travel path.', 'authored_fixture'),
      finding('sample-2', 8, 'The doorway warrants a physical width check.', 'authored_fixture'),
      finding('sample-3', 12, 'A floor transition warrants closer inspection.', 'authored_fixture')
    ]};
  }
  function localVideo(name) {
    return {source_kind: 'local_video', source_name: name, findings: []};
  }
  function review(state, id, status) {
    if (!statuses.includes(status)) throw new Error('Unknown review status.');
    if (!state.findings.some(item => item.id === id)) throw new Error('Unknown finding.');
    return {...state, findings: state.findings.map(item => item.id === id ? {...item, status} : {...item})};
  }
  function report(state, created_at = new Date().toISOString()) {
    return {schema_version: 1, tool: 'CAREC accessibility review pilot', created_at,
      source_kind: state.source_kind, source_name: state.source_name,
      analysis_mode: 'human_review', automatic_inference: false,
      limitations: ['Sample observations are authored examples, not AI detections.',
        'No verified distance, clearance, slope, passability, or safety assessment.',
        'Confirmation records reviewer opinion; it is not certification.'],
      findings: state.findings.map(item => ({...item}))};
  }
  const api = {finding, sample, localVideo, review, report};
  if (typeof module !== 'undefined' && module.exports) module.exports = api;
  else root.CARECPilot = api;
})(typeof globalThis !== 'undefined' ? globalThis : this);
