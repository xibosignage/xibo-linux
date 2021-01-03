#! /bin/bash

git tag -d edge
git push origin :edge
git tag edge
git push origin edge