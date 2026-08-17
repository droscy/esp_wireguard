#!/usr/bin/env bash
#
# Check the archive produced by `compote component pack` against the
# files.exclude rules in idf_component.yml.
#
# Packing on its own only proves the manifest parses. This additionally proves
# the exclude rules do something, and that the files a consumer actually needs
# survived them.
set -euo pipefail

dist_dir="${1:-dist}"

if ! archives=$(find "${dist_dir}" -maxdepth 1 -name '*.tgz' -type f); then
    echo "::error::failed to list ${dist_dir}"
    exit 1
fi

archive_list=()
if [ -n "${archives}" ]; then
    mapfile -t archive_list <<< "${archives}"
fi

if [ "${#archive_list[@]}" -ne 1 ]; then
    echo "::error::expected exactly one archive in ${dist_dir}, found ${#archive_list[@]}"
    exit 1
fi

archive="${archive_list[0]}"
echo "Checking ${archive}"

if ! contents=$(tar -tzf "${archive}"); then
    echo "::error::failed to read ${archive}"
    exit 1
fi

# tar entries may or may not carry a leading "./", so compare on a normalised
# copy rather than on the raw listing.
normalised=$(printf '%s\n' "${contents}" | sed 's|^\./||')

status=0

# Paths the manifest excludes. A published component carrying these would mean
# a files.exclude rule stopped matching.
excluded_prefixes=(
    ".github/"
    "tests/"
)
excluded_files=(
    "TODO.md"
)

for prefix in "${excluded_prefixes[@]}"; do
    matches=$(printf '%s\n' "${normalised}" | grep "^${prefix}" || true)
    if [ -n "${matches}" ]; then
        echo "::error::${prefix} should be excluded but the archive contains:"
        printf '%s\n' "${matches}" | sed 's/^/  /'
        status=1
    fi
done

for file in "${excluded_files[@]}"; do
    if printf '%s\n' "${normalised}" | grep -qx "${file}"; then
        echo "::error::${file} should be excluded but is present in the archive"
        status=1
    fi
done

# The exclude rules must not have taken the component with them. These are the
# files a consumer cannot build without.
required_files=(
    "CMakeLists.txt"
    "idf_component.yml"
    "include/esp_wireguard.h"
    "src/esp_wireguard_err.h"
    "src/esp_wireguard.c"
)

for file in "${required_files[@]}"; do
    if ! printf '%s\n' "${normalised}" | grep -qx "${file}"; then
        echo "::error::${file} is missing from the archive"
        status=1
    fi
done

if [ "${status}" -ne 0 ]; then
    echo "Archive contents:"
    printf '%s\n' "${normalised}" | sed 's/^/  /'
    exit "${status}"
fi

echo "Archive contents match the files.exclude rules in idf_component.yml"
