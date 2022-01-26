---
title: Release notes of the 3.3.4 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
date: 14/01/2022
lang: en-EN
numbersections: true
documentclass: article
from: markdown+tex_math_single_backslash
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Issues fixed

## Issue #80: [mfront] The variable seq is defined twice by the `MohrCoulomb` stress criterion in case of non-associated flow

Thig bug was only triggered in the `3.3.x` series if `C++-17` support was enabled.

For more details, see <https://github.com/thelfer/tfel/issues/80>