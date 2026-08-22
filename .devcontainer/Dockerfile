FROM osrf/ros:jazzy-desktop-full

ARG USERNAME=carec
ARG USER_UID=1000
ARG USER_GID=1000

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       clang-format \
       clang-tidy \
       git \
       python3-pip \
       python3-pytest \
       python3-venv \
       ros-jazzy-navigation2 \
       ros-jazzy-nav2-bringup \
       ros-jazzy-ros-gz \
       ros-jazzy-slam-toolbox \
    && rm -rf /var/lib/apt/lists/* \
    && groupadd --gid ${USER_GID} ${USERNAME} \
    && useradd --uid ${USER_UID} --gid ${USER_GID} -m ${USERNAME}

USER ${USERNAME}
WORKDIR /workspaces/CAREC-Project
