FROM ghcr.io/cirruslabs/android-sdk:35

ARG GRADLE_VERSION=8.10.2
ARG ANDROID_NDK_VERSION=27.2.12479018
ARG ANDROID_CMAKE_VERSION=3.22.1

ENV GRADLE_HOME=/opt/gradle \
    PATH=/opt/gradle/bin:${PATH}

RUN set -eux; \
    curl -fsSL "https://services.gradle.org/distributions/gradle-${GRADLE_VERSION}-bin.zip" -o /tmp/gradle.zip; \
    unzip -q /tmp/gradle.zip -d /opt; \
    mv "/opt/gradle-${GRADLE_VERSION}" /opt/gradle; \
    rm /tmp/gradle.zip; \
    yes | sdkmanager --licenses >/dev/null; \
    sdkmanager "ndk;${ANDROID_NDK_VERSION}" "cmake;${ANDROID_CMAKE_VERSION}"
