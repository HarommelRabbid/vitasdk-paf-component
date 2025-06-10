
#ifdef __SNC__
#include <kernel.h>
#else
#include <psp2/kernel/clib.h>
#endif

#include <paf.h>
#include <paf_web_map_view.h>


#ifndef PAF_BUILD_ASSERT
#define PAF_BUILD_ASSERT(condition) ((void)sizeof(char[1 - (2*(!(condition)))]))
#endif


int paf_sample_main(void){

	sceClibPrintf("VitaSDK build assert\n");

	PAF_BUILD_ASSERT(1 == sizeof(bool));
	PAF_BUILD_ASSERT(1 == sizeof(char));
	PAF_BUILD_ASSERT(2 == sizeof(short));
	PAF_BUILD_ASSERT(4 == sizeof(int));

	PAF_BUILD_ASSERT(0xC  == sizeof(paf::list<int>));
	PAF_BUILD_ASSERT(0x8  == sizeof(paf::pair<int, int>));
	PAF_BUILD_ASSERT(0x10 == sizeof(paf::pair<int, uint64_t>));
	PAF_BUILD_ASSERT(0xC  == sizeof(paf::map<int, int>));
	PAF_BUILD_ASSERT(0x10 == sizeof(paf::vector<int>));

	PAF_BUILD_ASSERT(0x10 == sizeof(paf::math::v1));
	PAF_BUILD_ASSERT(0x8  == sizeof(paf::math::v2));
	PAF_BUILD_ASSERT(0x10 == sizeof(paf::math::v3));
	PAF_BUILD_ASSERT(0x10 == sizeof(paf::math::v4));

	PAF_BUILD_ASSERT(0xC == sizeof(paf::string));
	PAF_BUILD_ASSERT(0xC == sizeof(paf::wstring));

	PAF_BUILD_ASSERT(0x10 == sizeof(paf::IDParam));
	PAF_BUILD_ASSERT(0x20 == sizeof(paf::VersatileParam));

	PAF_BUILD_ASSERT(0x20 == sizeof(paf::ui::FactoryFuncTable));
	PAF_BUILD_ASSERT(0xC == sizeof(paf::ui::FactoryFuncDrawObj));

	{ /* DrawObj */
		PAF_BUILD_ASSERT(0x50  == sizeof(paf::graph::DrawObj));
		PAF_BUILD_ASSERT(0xD8  == sizeof(paf::graph::PlaneObj));
	}

	{ /* DrawObjParam */
		PAF_BUILD_ASSERT(0x50  == sizeof(paf::graph::DrawObjParam));
		PAF_BUILD_ASSERT(0xE8  == sizeof(paf::graph::PlaneObjParam));
		PAF_BUILD_ASSERT(0x108 == sizeof(paf::graph::TextObjParam));
	}

	{ /* WidgetParam */
		PAF_BUILD_ASSERT(0x78 == sizeof(paf::ui::WidgetParam));
		PAF_BUILD_ASSERT(0x78 == sizeof(paf::ui::AppIconBaseParam));
		PAF_BUILD_ASSERT(0x78 == sizeof(paf::ui::BusyIndicatorParam));
		PAF_BUILD_ASSERT(0xE8 == sizeof(paf::ui::SlideBarParam));
		PAF_BUILD_ASSERT(0xE0 == sizeof(paf::ui::WebViewParam));
	}

	{ /* WidgetHandlerParam */
		PAF_BUILD_ASSERT(0x34 == sizeof(paf::ui::WidgetHandlerParam));
	}

	{ /* WidgetSoundParam */
		PAF_BUILD_ASSERT(0x3C == sizeof(paf::ui::WidgetSoundParam));
		PAF_BUILD_ASSERT(0x3C == sizeof(paf::ui::SlideBarSoundParam));
	}

	{ /* WidgetLayoutParam */
		PAF_BUILD_ASSERT(0xA8 == sizeof(paf::ui::WidgetLayoutParam));
	}

	{ /* WidgetStyleParam */
		PAF_BUILD_ASSERT(0x60 == sizeof(paf::ui::WidgetStyleParam));
		PAF_BUILD_ASSERT(0x78 == sizeof(paf::ui::BusyIndicatorStyleParam));
		PAF_BUILD_ASSERT(0x60 == sizeof(paf::ui::AppIconBaseStyleParam));
		PAF_BUILD_ASSERT(0x168 == sizeof(paf::ui::ScrollViewBaseStyleParam));
		PAF_BUILD_ASSERT(0x190 == sizeof(paf::ui::ScrollViewStyleParam));
		PAF_BUILD_ASSERT(0x410 == sizeof(paf::ui::ScrollBarStyleParam));
		PAF_BUILD_ASSERT(0x238 == sizeof(paf::ui::ScrollBar2dStyleParam));
		PAF_BUILD_ASSERT(0x180 == sizeof(paf::ui::ListViewStyleParam));
		PAF_BUILD_ASSERT(0x408 == sizeof(paf::ui::SlideBarStyleParam));
		PAF_BUILD_ASSERT(0x440 == sizeof(paf::ui::ButtonStyleParam));
		PAF_BUILD_ASSERT(0x440 == sizeof(paf::ui::StateButtonBaseStyleParam));
		PAF_BUILD_ASSERT(0x258 == sizeof(paf::ui::TextStyleParam));
		PAF_BUILD_ASSERT(0x60 == sizeof(paf::ui::WebViewStyleParam));
	}

	PAF_BUILD_ASSERT(0x28 == sizeof(paf::ui::Handler));

	{ /* Widget */
		PAF_BUILD_ASSERT(0x27C == sizeof(paf::ui::Widget));
		PAF_BUILD_ASSERT(0x280 == sizeof(paf::ui::Group));
		PAF_BUILD_ASSERT(0x2D0 == sizeof(paf::ui::AppIconBase));
		PAF_BUILD_ASSERT(0x2D0 == sizeof(paf::ui::AppIcon2d));
		PAF_BUILD_ASSERT(0x290 == sizeof(paf::ui::AppIconSimple));
		PAF_BUILD_ASSERT(0x280 == sizeof(paf::ui::Plane));
		PAF_BUILD_ASSERT(0x2A8 == sizeof(paf::ui::Supplier));
		PAF_BUILD_ASSERT(0x288 == sizeof(paf::ui::Canvas));
		PAF_BUILD_ASSERT(0x320 == sizeof(paf::ui::Box));
		PAF_BUILD_ASSERT(0x330 == sizeof(paf::ui::Dialog));
		PAF_BUILD_ASSERT(0x2A8 == sizeof(paf::ui::BusyIndicator));
		PAF_BUILD_ASSERT(0x2B0 == sizeof(paf::ui::ButtonBase));
		PAF_BUILD_ASSERT(0x2C0 == sizeof(paf::ui::Button));
		PAF_BUILD_ASSERT(0x2D0 == sizeof(paf::ui::CornerButton));
		PAF_BUILD_ASSERT(0x390 == sizeof(paf::ui::Camera));
		PAF_BUILD_ASSERT(0x2C8 == sizeof(paf::ui::StateButtonBase));
		PAF_BUILD_ASSERT(0x2C8 == sizeof(paf::ui::CheckBox));
		PAF_BUILD_ASSERT(0x280 == sizeof(paf::ui::CompositeDrawer));
		PAF_BUILD_ASSERT(0x2B0 == sizeof(paf::ui::CompositeButton));
		PAF_BUILD_ASSERT(0x2F8 == sizeof(paf::ui::DirectBar));
		PAF_BUILD_ASSERT(0x400 == sizeof(paf::ui::ScrollViewBase));
		PAF_BUILD_ASSERT(0x4E8 == sizeof(paf::ui::ListView));
		PAF_BUILD_ASSERT(0x4E8 == sizeof(paf::ui::FileList));
		PAF_BUILD_ASSERT(0x2D0 == sizeof(paf::ui::ListItem));
		PAF_BUILD_ASSERT(0x360 == sizeof(paf::ui::ImageButton));
		PAF_BUILD_ASSERT(0x2A8 == sizeof(paf::ui::ImageSlideBar));
		PAF_BUILD_ASSERT(0x2F0 == sizeof(paf::ui::SlideBar));
		PAF_BUILD_ASSERT(0x2C0 == sizeof(paf::ui::PlaneButton));
		PAF_BUILD_ASSERT(0x2F0 == sizeof(paf::ui::ProgressBar));
		PAF_BUILD_ASSERT(0x318 == sizeof(paf::ui::ProgressBarTouch));
		PAF_BUILD_ASSERT(0x290 == sizeof(paf::ui::RadioBox));
		PAF_BUILD_ASSERT(0x2C8 == sizeof(paf::ui::RadioButton));
		PAF_BUILD_ASSERT(0x550 == sizeof(paf::ui::RichText));
		PAF_BUILD_ASSERT(0x3C0 == sizeof(paf::ui::Scene));
		PAF_BUILD_ASSERT(0x440 == sizeof(paf::ui::ScrollView));
		PAF_BUILD_ASSERT(0x2D0 == sizeof(paf::ui::ScrollBar));
		PAF_BUILD_ASSERT(0x2D0 == sizeof(paf::ui::ScrollBar2d));
		PAF_BUILD_ASSERT(0x2E8 == sizeof(paf::ui::ScrollBarTouch));
		PAF_BUILD_ASSERT(0x430 == sizeof(paf::ui::SlidingDrawer));
		PAF_BUILD_ASSERT(0x2D8 == sizeof(paf::ui::Text));
		PAF_BUILD_ASSERT(0x328 == sizeof(paf::ui::TextBox));
		PAF_BUILD_ASSERT(0x2B0 == sizeof(paf::ui::SpinBase));
		PAF_BUILD_ASSERT(0x2BC == sizeof(paf::ui::ItemSpin)); // not sure
		PAF_BUILD_ASSERT(0x2B8 == sizeof(paf::ui::NumSpin));
		PAF_BUILD_ASSERT(0x2B0 == sizeof(paf::ui::PageTurnBase));
		PAF_BUILD_ASSERT(0x2F8 == sizeof(paf::ui::PageTurn));

		PAF_BUILD_ASSERT(0x830 == sizeof(paf::ui::WebView));
	}

/*

paf::ui::Widget::UserData
paf::ui::Widget::CreateParam
paf::ui::ClipRect
paf::ui::Camera::DrawScissorArg
paf::ui::PartsList
paf::ui::filelist::VisualizerBindParam
paf::ui::filelist::IDPair
paf::ui::filelist::CellFocusParam
paf::ui::filelist::Manager::InitParam
paf::ui::filelist::CellSource
paf::ui::filelist::SegmentSource
paf::ui::filelist::CellListAccessor
paf::ui::filelist::SegmentListAccessor
paf::ui::filelist::SegmentListAccessor::OptionParam
paf::ui::filelist::Visualizer
paf::ui::listview::ListPos
paf::ui::listview::SyncParam
paf::ui::listview::CellList
paf::ui::listview::CellMap
paf::ui::listview::SegmentList
paf::ui::listview::Segment
paf::ui::listview::Cell
paf::ui::listview::ItemFactory
paf::ui::listview::ItemFactory::CreateParam
paf::ui::listview::ItemFactory::StartParam
paf::ui::listview::ItemFactory::StopParam
paf::ui::listview::ItemFactory::DisposeParam
paf::ui::RichText::MemoryAllocator (protected)
paf::ui::ScrollViewSnapPoint
paf::ui::ScrollViewBase::Snap
paf::ui::ScrollViewParent
paf::ui::ScrollViewTimer
paf::ui::ImeParam

paf::sound::Output
paf::sound::Output::Sound (private)
paf::Buffer
paf::SubBuffer
paf::SubConstBuffer
paf::UnmanagedBuffer
paf::UnmanagedConstBuffer
paf::MallocBuffer
paf::HeapAllocBuffer
paf::cache::CacheBase
paf::cache::CacheBase::Option
paf::cache::ImageCache
paf::cache::ImageCache::Option
paf::cache::CacheManager
paf::cache::CacheManager::Params
paf::cache::CacheManagerImpl
paf::common::ArrayFunc<T>
paf::common::AveragedValue<T>
paf::common::CallList
paf::common::EnumFlag<T>
paf::common::JobCallQueue
paf::common::MainThreadCall
paf::common::property_list::PropertyValue
paf::common::property_list::PropertyElement
paf::common::property_list::StringElement
paf::common::property_list::WStringElement
paf::common::PropertyList
paf::common::RefObj
paf::common::detail::RefCounter<T>
paf::common::detail::SharedPtr<T>
paf::common::detail::WeakPtr<T>
paf::common::String
paf::common::String16
paf::common::StaticLog<T>
paf::common::StaticStack<T, CAP>
paf::common::Tokenizer
paf::cxml::Header
paf::cxml::Document
paf::cxml::Element
paf::cxml::Attribute
paf::cxml::File
paf::datetime::DateTime
paf::DirEnt
paf::Dir
paf::BufferedFile
paf::BufferedFile::OpenArg
paf::FileStat
paf::File
paf::File::OpenArg
paf::HttpFile
paf::HttpFile::OpenArg
paf::HttpFileStat
paf::LocalFile
paf::LocalFile::OpenArg
paf::LocalFileImpl
paf::LocalFilePsp2Impl
paf::LocalFileStat
paf::MemFile
paf::MemFile::OpenArg
paf::graph::arl::ShaderPatcher
paf::graph::arl::BlendInfo
paf::graph::arl::cached_states
paf::graph::arl::ArlInstance
paf::graph::BasicShader 
paf::graph::DrawCallbackArg
paf::graph::DeleteCBInfo
paf::graph::DrawStyle
paf::graph::DrawObjParam
paf::graph::DrawObj
paf::graph::DrawEnv
paf::graph::PlaneObj
paf::graph::ShaderDrawArg
paf::graph::Shader
paf::graph::TextObj
paf::GraphMem::GraphHeapAllocator
paf::GraphMem::Option
paf::graph::SurfaceCLUT
paf::graph::Surface
paf::graph::Surface::Texture
paf::graph::Surface::LoadOption
paf::graph::Surface::SysMemory (protected)
paf::graph::Surface::TexParam (protected)
paf::graph::SurfaceListNode
paf::graph::SurfaceBase
paf::graph::DisplaySurface
paf::graph::SurfacePoolAllocator
paf::graph::SurfacePoolAllocator_Heap
paf::graph::SurfacePool
paf::graph::SurfacePool::Option
paf::graph::ScreenSurface
paf::graph::TextStyleRun
paf::graph::AttributedString
paf::graph::TextLayoutCharInfo
paf::graph::TextLayoutLineInfo
paf::graph::TextStyle
paf::gutil::WinHandle
paf::gutil::DevInitParam
paf::gutil::InitParam
paf::image::ImageSource
paf::image::ImageOperation
paf::image::Image
paf::image::ImageArray
paf::image::ImageAttribute
paf::image::ImageExtent
paf::image::ImagePoint
paf::image::ImageRect
paf::image::ImageBuffer
paf::inputdevice::motionsensor::Data
paf::inputdevice::pad::InitParam
paf::inputdevice::pad::Data
paf::inputdevice::touchscreen::InitParam
paf::inputdevice::touchscreen::Data
paf::inputdevice::touchscreen::DeviceInfo
paf::inputdevice::InitParam
paf::inputdevice::Data
paf::inputdevice::InputListener
paf::inputdevice::GestureParam
paf::inputdevice::GestureListener
paf::inputdevice::GestureListenerPack
paf::inputdevice::InputGestureListener
paf::job::DefaultPriority
paf::job::JobItem
paf::job::JobItem::Option
paf::job::JobQueue
paf::job::JobQueue::Option
<paf::math>
paf::memory::MemoryAllocator
paf::memory::HeapAllocator
paf::memory::HeapAllocator::Option
paf::memory::HeapAllocator::HeapInfo
paf::Module
paf::Module::ExtraOption
paf::ModuleImpl
paf::resource::ResourceObj
paf::resource::ResourceObj::Option
<paf::std>
paf::thread::CallQueue
paf::thread::Cond
paf::thread::Mutex
paf::thread::RMutex
paf::thread::RWLock
paf::thread::Semaphore
paf::thread::SyncCall
paf::thread::Thread
paf::thread::Thread::Option
paf::RawTimer
paf::Timer
paf::Timer2D
paf::Timer3D
paf::TimerListener
paf::TimerListenerList
paf::TimerListenerList::ListenerParam
paf::TimerQueue
paf::Url
paf::ResourceParam
paf::ResourceParam::SemanticParam
paf::VisualParam
paf::VisualParam::ParamBase
paf::VisualParam::ParamWithDirtyFlag<T>
paf::VisualParam::MinMaxParam<T>
paf::VisualParam::BoolParam
paf::VisualParam::FloatParam
paf::VisualParam::IntParam
paf::VisualParam::Vec2Param
paf::VisualParam::Vec3Param
paf::VisualParam::Vec4Param
paf::VisualParam::IdParamParam
paf::VisualParam::TextureParam
paf::VersatileParamManager
paf::ui::AnimListener
paf::ui::AnimPropertyV4
paf::ui::AnimListenerV4
paf::ui::AnimPropertyF
paf::ui::AnimListenerF1
paf::ui::AnimListenerObj
paf::ui::TexAnimListenerObj
paf::ui::AnimQueueBase
paf::ui::AnimQueue
paf::ui::Screen
paf::ui::EnvironmentParam
paf::ui::Environment
paf::ui::FocusInfo
paf::ui::FocusInfo::History
paf::ui::Event
paf::ui::EventListener
paf::ui::EventListenerList
paf::ui::EventQueue
paf::ui::EventCBListener
paf::ui::TimeoutListener
paf::ui::Layouter
paf::ui::Animator
paf::ui::UIInputGestureListener
paf::ui::WidgetStyleParam
paf::ui::wutil::InitParam
paf::Framework
paf::Framework::InitParam
paf::Framework::CommonResourceInitParam
paf::cdlg::OnUpdateData
paf::DrawOrderElement
paf::InitEventFunctionEntry
paf::InitEventFunctionEntry2
paf::WidgetResourceInfo
paf::WidgetResourceInfo::LayoutData
paf::Plugin
paf::Plugin::Argument == 0x14
paf::Plugin::AddTextureInfo
paf::Plugin::InitParam2
paf::Plugin::InitParam
paf::Plugin::PageOpenParam
paf::Plugin::PageCloseParam
paf::Plugin::TemplateOpenParam

*/

	exit(0);

	return 0;
}
