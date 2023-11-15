#!/usr/bin/env bash
# set -e

gem build Rdmtx.gemspec
gem install ./Rdmtx-*.gem
ruby test/test_*.rb
