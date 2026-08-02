# CLAUDE.md — 2110366 EMBEDDED SYS LAB (2026-1)

Course-specific instructions for Claude Code. The repo-wide `CLAUDE.md` at the repo
root still applies — read it too if you haven't this session.

## About this course

> Fill this in as the semester starts (or ask Claude to draft it from `0_Meta/syllabus.md`
> once that's populated): what the course covers, instructor, format, anything that
> helps answer questions well.

- **Code:** 2110366
- **Name:** EMBEDDED SYS LAB
- **Semester:** 2026-1
- **Instructor:** _fill in_
- **NotebookLM notebook ID:** see `.env` in this folder (`NOTEBOOKLM_NOTEBOOK_ID`)

## ⚠️ Keep this file in sync with the folder

This file must stay in sync with what's actually in this course folder — especially
`0_Meta/syllabus.md` and `0_Meta/assignments.md`. Whenever you add or substantially
change syllabus content, grading breakdown, key topics, or important dates, update the
relevant section below in the same turn.

## Key topics / schedule

> Summarize the syllabus's topic list / weekly schedule here once known, so a quick
> glance (without opening the full syllabus) is useful. Keep this a summary, not a copy
> — link to `0_Meta/syllabus.md` for the full text.

## Folder map

This course folder lives at `$COURSES_DIR/2026-1/embedded-sys-lab/`.

| Folder | What's there |
|---|---|
| `0_Meta/` | syllabus, assignments/due-dates tracker |
| `1_Slides/` | PDF slides, including handwritten ones |
| `2_Transcripts/` | lecture transcripts + a short summary per lecture |
| `3_Notes/` | hand-written notes — never auto-generate into here |
| `4_References/` | one `.md` per external reference (see `_template_reference.md`) |
| `8_Exercises/` | exercises / project files |
| `9_Exam/` | exam prep notes, quiz prep |

## Using NotebookLM for this course

This course's NotebookLM notebook is grounded in whatever's been uploaded via
`setup/per_course/sync.sh`. Prefer it for anything that benefits from being grounded in
the actual lecture material rather than general knowledge:

```bash
notebooklm use "$NOTEBOOKLM_NOTEBOOK_ID"     # set from this folder's .env
notebooklm ask "..."
notebooklm generate quiz
notebooklm generate flashcards
```

If material in this folder is newer than what's in the notebook, run this from the repo
root first:

```bash
bash setup/per_course/sync.sh 2026-1 embedded-sys-lab
```

## Assignments / due dates

Tracked in `0_Meta/assignments.md`, synced with the shared Notion database (rows are
matched by this course's Course + Semester, so other semesters of the same course don't
collide):

```bash
.venv/bin/python3 setup/per_course/notion_assignments.py pull 2026-1 embedded-sys-lab   # refresh from Notion
.venv/bin/python3 setup/per_course/notion_assignments.py push 2026-1 embedded-sys-lab   # push local edits
```

When asked "what's due" for this course, read `0_Meta/assignments.md` directly rather
than guessing — pull first if it might be stale.
