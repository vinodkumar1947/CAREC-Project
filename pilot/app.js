'use strict';
const api = CARECPilot;
let state = api.sample();
let videoUrl = null;
let nextId = 1;
const el = id => document.getElementById(id);
const message = text => { el('message').textContent = text; };
function render() {
  el('source-name').textContent = state.source_name;
  el('mode').textContent = state.source_kind === 'synthetic_sample' ? 'AUTHORED SAMPLE' : 'MANUAL REVIEW';
  el('count').textContent = `${state.findings.filter(x => x.status !== 'unreviewed').length} / ${state.findings.length} reviewed`;
  const list = el('findings');
  list.replaceChildren();
  if (!state.findings.length) {
    const empty = document.createElement('p');
    empty.textContent = 'No observations yet. Play your video and add a note at a relevant timestamp. Automatic detection is not implemented.';
    list.append(empty);
  }
  state.findings.forEach(item => {
    const row = document.createElement('article'); row.className = 'finding';
    const jump = document.createElement('button'); jump.type = 'button'; jump.className = 'time-button';
    jump.textContent = `${item.timestamp_s.toFixed(1)}s · ${item.status}`;
    jump.setAttribute('aria-label', `Review observation at ${item.timestamp_s} seconds, ${item.status}`);
    jump.onclick = () => {
      el('timestamp').value = item.timestamp_s;
      if (state.source_kind === 'local_video') { el('video').pause(); el('video').currentTime = item.timestamp_s; }
      message(state.source_kind === 'synthetic_sample' ? `Selected authored example at ${item.timestamp_s}s. The illustration is static.` : `Paused at ${item.timestamp_s}s.`);
    };
    const title = document.createElement('h3'); title.textContent = item.description;
    const origin = document.createElement('small'); origin.textContent = item.origin === 'authored_fixture' ? 'Authored example · not an AI detection' : 'Human-authored observation';
    const actions = document.createElement('div'); actions.className = 'actions';
    for (const [status, label] of [['confirmed','Confirm'],['dismissed','Dismiss'],['unreviewed','Reset review']]) {
      const button = document.createElement('button'); button.type = 'button'; button.textContent = label;
      button.setAttribute('aria-label', `${label}: ${item.description}`);
      button.setAttribute('aria-pressed', String(item.status === status));
      button.onclick = () => {
        state = api.review(state, item.id, status); render();
        // Preserve keyboard focus when the row is rebuilt.
        list.querySelector(`[data-id="${item.id}"]`).querySelector(`[data-status="${status}"]`).focus();
        message(`Observation ${status}. Export to keep your review.`);
      };
      button.dataset.status = status; actions.append(button);
    }
    row.dataset.id = item.id; row.append(jump, title, origin, actions); list.append(row);
  });
}
function discardAllowed() {
  return !state.findings.some(x => x.origin === 'manual' || x.status !== 'unreviewed') || window.confirm('Switching sources discards this review. Cancel and export first if you want to keep it.');
}
function releaseVideo() {
  el('video').pause(); el('video').removeAttribute('src'); el('video').load();
  if (videoUrl) URL.revokeObjectURL(videoUrl); videoUrl = null;
}
el('sample').onclick = () => {
  if (!discardAllowed()) return;
  releaseVideo(); state = api.sample(); el('video-file').value = '';
  el('video').hidden = true; el('illustration').hidden = false;
  el('preview-note').textContent = 'The sample timestamps describe a hypothetical 15-second walkthrough.';
  el('timestamp').value = 0; render(); message('Authored sample loaded. No AI analysis has run.');
};
el('video-file').onchange = event => {
  const file = event.target.files[0]; if (!file) return;
  if (!discardAllowed()) { event.target.value = ''; return; }
  releaseVideo(); state = api.localVideo(file.name);
  videoUrl = URL.createObjectURL(file); el('video').src = videoUrl;
  el('video').hidden = false; el('illustration').hidden = true;
  el('timestamp').value = 0;
  el('preview-note').textContent = 'Local playback only. Add observations manually. Your file is not uploaded. Refreshing clears this session.';
  render(); message('Local video selected. Sample observations have been cleared.');
};
el('video').onerror = () => message('This browser could not play the video. Try a browser-supported MP4 or WebM file. No analysis has run.');
el('video').ontimeupdate = () => { if (document.activeElement !== el('timestamp')) el('timestamp').value = el('video').currentTime.toFixed(1); };
el('observation-form').onsubmit = event => {
  event.preventDefault();
  try {
    const timestamp = Number(el('timestamp').value);
    if (state.source_kind === 'local_video' && (!Number.isFinite(el('video').duration) || timestamp > el('video').duration)) throw new Error('Wait for playable video metadata and choose a timestamp within the video.');
    if (state.source_kind === 'synthetic_sample' && timestamp > 15) throw new Error('Use a timestamp within the 15-second sample.');
    state.findings.push(api.finding(`manual-${nextId++}`, timestamp, el('description').value));
    state.findings.sort((a,b) => a.timestamp_s - b.timestamp_s);
    el('description').value = ''; render(); message('Observation added. Export to keep it.');
  } catch (error) { message(error.message); }
};
el('export').onclick = () => {
  const url = URL.createObjectURL(new Blob([JSON.stringify(api.report(state), null, 2)], {type:'application/json'}));
  const anchor = document.createElement('a'); anchor.href = url; anchor.download = 'carec-review.json';
  document.body.append(anchor); anchor.click(); anchor.remove(); setTimeout(() => URL.revokeObjectURL(url), 1000);
  message('Report exported. It contains the source filename and your notes, but no video bytes. Review before sharing.');
};
el('print').onclick = () => window.print();
window.addEventListener('beforeunload', event => {
  if (state.findings.some(x => x.origin === 'manual' || x.status !== 'unreviewed')) { event.preventDefault(); event.returnValue = ''; }
});
render();
