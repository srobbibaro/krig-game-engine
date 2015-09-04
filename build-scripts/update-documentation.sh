#!/bin/bash

if [ "$TRAVIS_PULL_REQUEST" == "false" ] && [ "$TRAVIS_BRANCH" == "master" ]; then
  git config --global user.email "${GH_EMAIL}"
  git config --global user.name "travis-ci"
  git clone --quiet -b gh-pages https://${GH_TOKEN}@github.com/srobbibaro/krig-game-engine.git html > /dev/null 2>&1

  rm -r html/search/
  rm  html/*
  make doc > /dev/null 2>&1
  cd html

  git commit -am "Update documentation after successful master build: $TRAVIS_BUILD_NUMBER"
  git push -fq origin gh-pages > /dev/null 2>&1
fi
