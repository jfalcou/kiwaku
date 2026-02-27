#!/bin/bash
# KIWAKU - Containers Well Made
# Copyright : KIWAKU Project Contributors
# SPDX-License-Identifier: BSL-1.0
sed 's/_::[a-zA-Z0-9_<>]*/kwk_implementation_defined/g' "$1"