
function build_sample(){

  export PAF_SAMPLE_NAME=$1

  if [ ! -d build_$1 ]; then
    mkdir build_$1
  fi

  cd build_$1
  cmake ..
  if [ $? -ne 0 ]; then
    exit 1
  fi
  make -j$(nproc)
  if [ $? -ne 0 ]; then
    exit 1
  fi
  cd ..
}

build_sample paf_build_assert_vitasdk

build_sample paf_sample_cxml_plane
build_sample paf_sample_cxml_app_icon_simple
build_sample paf_sample_cxml_text
build_sample paf_sample_cxml_rich_text
build_sample paf_sample_cxml_busyindicator
build_sample paf_sample_cxml_progressbar
build_sample paf_sample_cxml_progressbar_touch
build_sample paf_sample_cxml_slidebar
build_sample paf_sample_cxml_button
build_sample paf_sample_cxml_corner_button
build_sample paf_sample_cxml_speech_balloon
build_sample paf_sample_cxml_num_spin
build_sample paf_sample_cxml_list_view-list_item
build_sample paf_sample_cxml_text_box
build_sample paf_sample_cxml_check_box
build_sample paf_sample_cxml_webview
build_sample paf_sample_cxml_radio_box-radio_button

build_sample paf_sample_advanced_transition

build_sample paf_sample_feature_screen_orientation
build_sample paf_sample_feature_input_listener
build_sample paf_sample_feature_image_to_texture
build_sample paf_sample_feature_thread_task_job
