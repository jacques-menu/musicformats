#!/bin/zsh

#set -x

function usage ()
  {
    echo "Parameter error:"
    echo "  '"$1"'" "is unknown"
    echo "Usage is:"
    echo "  $0 [cmake] [clean] [install]"
    echo
    exit 1
  }

#	my actual work directory
#----------------------------------------------

MY_WORK_DIR=${HOME}


# dev branch
#----------------------------------------------

MUSIC_FORMATS_DEV=${MY_WORK_DIR}/musicformats-git-dev
# echo
# echo "==> MUSIC_FORMATS_DEV = ${MUSIC_FORMATS_DEV}"
# echo


# Write all output to logfile
# -----------------------------------------

SCRIPT_BASE_NAME="$(basename ${0})"

LOGFILE_NAME=${SCRIPT_BASE_NAME/%.zsh/.zsh.log}
LOGFILE=${MUSIC_FORMATS_DEV}/build/${LOGFILE_NAME}
# echo "==> $0: 0 = ${0}"
# echo "==> $0: SCRIPT_BASE_NAME = ${SCRIPT_BASE_NAME}"
# echo "==> $0: LOGFILE_NAME     = ${LOGFILE_NAME}"
# echo "==> $0: LOGFILE          = ${LOGFILE}"
# echo

exec > ${LOGFILE} 2>&1


# Let's go!
# -----------------------------------------

gh auth login jacques-menu

owner=GITHUB_REPO_OWNER
repo=GITHUB_REPO_NAME

gh api \
  repos/\(owner/\)repo/actions/runs \
  | jq -r '.workflow_runs[] | select(.head_branch != "master") | "(.id)"' \
  | xargs -n1 -I %

gh api repos/\(owner/\)repo/actions/runs/% -X DELETE
#Successfully installed blackini

cat tox.ini
#  [tool.black]
#  line-length = 79
#  target-version = py36,
black --config-file=tox.ini

#set +x
