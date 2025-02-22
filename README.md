PPSSPP - a fast and portable PSP emulator
=========================================

Created by Henrik Rydgård

Additional code by many contributors, see the Credits screen

Originally released under the GPL 2.0 (and later) in November 2012

Official website:
https://www.ppsspp.org/

Discord:
https://discord.gg/5NJB6dD

No BIOS file required to play, PPSSPP is an "HLE" emulator.  Default settings balance good compatibility and speed.

To contribute, see [the development page](https://www.ppsspp.org/development.html).  Help testing, investigating, or fixing is always welcome.  See [the list of issues](https://github.com/hrydgard/ppsspp/issues).

For the latest source code, see [our GitHub page](https://github.com/hrydgard/ppsspp).

For build instructions and other development tutorials, see [the wiki](https://github.com/hrydgard/ppsspp/wiki).

If you want to download regularly updated builds for Android, Windows x86 and x64, proceed to this [page](https://buildbot.orphis.net/ppsspp/)

For game compatibility, see [community compatibility feedback](https://report.ppsspp.org/games).

What's new in 1.10.3
--------------------
* Fix for control layout editor (#13125)

What's new in 1.10.2
--------------------
* More crashfixes ([#13094](https://github.com/hrydgard/ppsspp/issues/13094), [#13093](https://github.com/hrydgard/ppsspp/issues/13093))
* Improve download performance and cancel behavior ([#13095](https://github.com/hrydgard/ppsspp/issues/13095))
* Restore the removed I/O on Thread option ([#13096](https://github.com/hrydgard/ppsspp/issues/13096))

What's new in 1.10.1
--------------------
* Fixes for common crashes ([#13077](https://github.com/hrydgard/ppsspp/issues/13077), [#13076](https://github.com/hrydgard/ppsspp/issues/13076), see [#13057](https://github.com/hrydgard/ppsspp/issues/13057))
* Fix for offset rendering in D3D9 ([#13071](https://github.com/hrydgard/ppsspp/issues/13071))

What's new in 1.10.0
--------------------
* Graphics and compatibility fixes ([#12800](https://github.com/hrydgard/ppsspp/issues/12800), [#12670](https://github.com/hrydgard/ppsspp/issues/12670), [#12635](https://github.com/hrydgard/ppsspp/issues/12635), [#12857](https://github.com/hrydgard/ppsspp/issues/12857), [#12941](https://github.com/hrydgard/ppsspp/issues/12941), [#11898](https://github.com/hrydgard/ppsspp/issues/11898), [#12695](https://github.com/hrydgard/ppsspp/issues/12695), more)
* Assorted minor performance improvements, game load speedup ([#12462](https://github.com/hrydgard/ppsspp/issues/12462), [#12652](https://github.com/hrydgard/ppsspp/issues/12652))
* Screen inset (notch) support on Android ([#12779](https://github.com/hrydgard/ppsspp/issues/12779))
* Analog stick support for menu navigation ([#12685](https://github.com/hrydgard/ppsspp/issues/12685))
* Fixed audio glitches in SDL builds ([#12916](https://github.com/hrydgard/ppsspp/issues/12916), [#12920](https://github.com/hrydgard/ppsspp/issues/12920))
* Support more languages in in-game dialogs ([#12702](https://github.com/hrydgard/ppsspp/issues/12702)). Croatian language added to PPSSPP.
* Simple multiplayer chat ([#12667](https://github.com/hrydgard/ppsspp/issues/12667))
* More advanced postprocessing (multipass, parameters) ([#12905](https://github.com/hrydgard/ppsspp/issues/12905), [#12901](https://github.com/hrydgard/ppsspp/issues/12901))
* Add PPSSPP-specific CWCheat ([#12816](https://github.com/hrydgard/ppsspp/issues/12816), [#12912](https://github.com/hrydgard/ppsspp/issues/12912))
* Reintroduce Cardboard VR, allow more resolutions ([#12449](https://github.com/hrydgard/ppsspp/issues/12449), [#8714](https://github.com/hrydgard/ppsspp/issues/8714))
* Fix some crashes ([#12908](https://github.com/hrydgard/ppsspp/issues/12908), [#12876](https://github.com/hrydgard/ppsspp/issues/12876))
* Ghost in the Shell graphics fixed (JIT inaccuracy with inf*0) ([#12519](https://github.com/hrydgard/ppsspp/issues/12519))
* Mac build now supports Vulkan on top of MoltenVK ([#12583](https://github.com/hrydgard/ppsspp/issues/12583))
* Raspberry Pi 4 EGL crash fixed ([#12474](https://github.com/hrydgard/ppsspp/issues/12474))
* VSync now supported on all backends, frame duplication option added for 30 Hz games ([#12659](https://github.com/hrydgard/ppsspp/issues/12659), [#12602](https://github.com/hrydgard/ppsspp/issues/12602))
* Camera supported on Windows, Linux and Mac (still no microphone though) ([#12572](https://github.com/hrydgard/ppsspp/issues/12572), [#12580](https://github.com/hrydgard/ppsspp/issues/12580), [#12607](https://github.com/hrydgard/ppsspp/issues/12607))
* Darkstalkers fixed and working through software rendering. SW rendering fixed on GLES 2.0 ([#12443](https://github.com/hrydgard/ppsspp/issues/12443), [#12898](https://github.com/hrydgard/ppsspp/issues/12898))
* Hot Shots Golf slowdown and flicker on Vulkan fixed ([#12873](https://github.com/hrydgard/ppsspp/issues/12873), [#12746](https://github.com/hrydgard/ppsspp/issues/12746))
* Pangya Golf crashes and hangs fixed ([#12718](https://github.com/hrydgard/ppsspp/issues/12718))
* Allow rebinding of right touch screen analog ([#12486](https://github.com/hrydgard/ppsspp/issues/12486))
* Add option to prevent mipmaps from being dumped ([#12818](https://github.com/hrydgard/ppsspp/issues/12818))
* Tilt control now have a base radius to help with deadzone ([#12756](https://github.com/hrydgard/ppsspp/issues/12756))
* Mappable auto rotating analog stick to pass some game checks ([#12749](https://github.com/hrydgard/ppsspp/issues/12749))
* Touch control position can now be snapped to a grid ([#12517](https://github.com/hrydgard/ppsspp/issues/12517))
* HiDPI retina display support ([#12552](https://github.com/hrydgard/ppsspp/issues/12552))
* Rapid-fire on touch control ([#12601](https://github.com/hrydgard/ppsspp/issues/12601))
* Toggle mute button ([#12643](https://github.com/hrydgard/ppsspp/issues/12643))
* Add option to resize game icons and more ([#12646](https://github.com/hrydgard/ppsspp/issues/12646), [#12637](https://github.com/hrydgard/ppsspp/issues/12637))
* Frames in-flight now configurable to reduce input lag at the cost of speed ([#12660](https://github.com/hrydgard/ppsspp/issues/12660))
* Add toggle mode to combo button ([#12623](https://github.com/hrydgard/ppsspp/issues/12623))
* SDL mouse support, Qt menu upgrades ([#12612](https://github.com/hrydgard/ppsspp/issues/12612), [#12817](https://github.com/hrydgard/ppsspp/issues/12817))
* Real support for chinese patched version of Hatsune Miku Project Diva Extend ([#13007](https://github.com/hrydgard/ppsspp/issues/13007))
* Some minor kernel module support ([#13028](https://github.com/hrydgard/ppsspp/issues/13028), [#12225](https://github.com/hrydgard/ppsspp/issues/12225), [#13026](https://github.com/hrydgard/ppsspp/issues/13026), [#13004](https://github.com/hrydgard/ppsspp/issues/13004), [#13038](https://github.com/hrydgard/ppsspp/issues/13038), [#13023](https://github.com/hrydgard/ppsspp/issues/13023))
* Fixed fullscreen toggling with Vulkan in SDL builds ([#11974](https://github.com/hrydgard/ppsspp/issues/11974))

What's new in 1.9.0/1.9.3
-------------------------
* Sizing/touch fixes on Android
* Fixes for some crashes

What's new in 1.9.0/1.9.2
-------------------------
* Flicker fixed in God of War that appeared with newer drivers for Mali GPUs ([#12224](https://github.com/hrydgard/ppsspp/issues/12224))
* Improve performance of God of War on Vulkan ([#12242](https://github.com/hrydgard/ppsspp/issues/12242)), implement built-in GPU profiler ([#12262](https://github.com/hrydgard/ppsspp/issues/12262), [#12266](https://github.com/hrydgard/ppsspp/issues/12266))
* Vertex range culling fixed on ARM Mali ([#12227](https://github.com/hrydgard/ppsspp/issues/12227))
* Started to improve VFPU precision, resulting so far in a fix for the long standing Tekken 6 leg shaking problem ([#12217](https://github.com/hrydgard/ppsspp/issues/12217))
* Fixed a VFPU precision snafu on ARM64, fixing disappearing officers in Warriors Orochi ([#11299](https://github.com/hrydgard/ppsspp/issues/11299)) and some problems in Tomb Raider ([#11179](https://github.com/hrydgard/ppsspp/issues/11179)).
* Vulkan is the default again on Android versions newer than Pie
* Fix various homebrew store issues
* GPU pause signal handling fixed, fixing some hangs in Bleach and Armored Core games ([#12160](https://github.com/hrydgard/ppsspp/issues/12160))
* Audio sample rate conversion handling fixes ([#12147](https://github.com/hrydgard/ppsspp/issues/12147))
* Some Vulkan optimizations (pre-rotation ([#12216](https://github.com/hrydgard/ppsspp/issues/12216)), perf fix in Metal Gear Acid, etc)
* Multiple fixes for the UWP build ([#12036](https://github.com/hrydgard/ppsspp/issues/12036), others)
* MP3 playback fixes ([#12005](https://github.com/hrydgard/ppsspp/issues/12005))
* Audio in Motorstorm: Arctic Edge fixed by returning errors correctly ([#12121](https://github.com/hrydgard/ppsspp/issues/12121))
* Audio glitches in Final Fantasy Tactics fixed ([#9956](https://github.com/hrydgard/ppsspp/issues/9956))
* Camera display in Invizimals fixed ([#12278](https://github.com/hrydgard/ppsspp/issues/12278), [#8521](https://github.com/hrydgard/ppsspp/issues/8521))
* Added hotkeys for texture dump and replace ([#11938](https://github.com/hrydgard/ppsspp/issues/11938))
* Added Visual Studio 2019 support. Windows XP is no longer supported ([#11995](https://github.com/hrydgard/ppsspp/issues/11995), others)
* Fixes for video capture ([#12069](https://github.com/hrydgard/ppsspp/issues/12069))
* Added a separate sound volume for alternative speed ([#12124](https://github.com/hrydgard/ppsspp/issues/12124))
* Improved mouse control (Windows only) ([#12173](https://github.com/hrydgard/ppsspp/issues/12173), [#12176](https://github.com/hrydgard/ppsspp/issues/12176))
* Support for installing texture packs and ISOs from zips ([#12175](https://github.com/hrydgard/ppsspp/issues/12175))
* Right analog support for touch controls (only used by patched games and some HD remasters) ([#12182](https://github.com/hrydgard/ppsspp/issues/12182))
* Android: Fix OpenSL initialization, possibly helps audio crackle a little. ([#12333](https://github.com/hrydgard/ppsspp/issues/12333)).
* Fix graphics on Amazon Fire TV Stick 4K
* Fixed strange vehicle behavior in MGS:PW (somehow) ([#12342](https://github.com/hrydgard/ppsspp/issues/12342))

What's new in 1.8.0
-------------------
* Avoid crash when starting PPSSPP due to bad AMD Vulkan drivers ([#11802](https://github.com/hrydgard/ppsspp/issues/11802))
* PowerVR Vulkan display corruption fixed ([#11744](https://github.com/hrydgard/ppsspp/issues/11744), [#11747](https://github.com/hrydgard/ppsspp/issues/11747))
* Naruto Shippuden 3 hang fixed ([#11780](https://github.com/hrydgard/ppsspp/issues/11780))
* Fixes to various lighting bugs ([#11567](https://github.com/hrydgard/ppsspp/issues/11567), [#11574](https://github.com/hrydgard/ppsspp/issues/11574), [#11577](https://github.com/hrydgard/ppsspp/issues/11577))
* Fix control issue in Sonic Rivals and Rock Band ([#11878](https://github.com/hrydgard/ppsspp/issues/11878), [#11798](https://github.com/hrydgard/ppsspp/issues/11798), [#11879](https://github.com/hrydgard/ppsspp/issues/11879))
* Significant performance improvement in Earth Defense Force 2 ([#11172](https://github.com/hrydgard/ppsspp/issues/11172))
* Fix "real clock sync" setting (helps with latency for music games - [#11813](https://github.com/hrydgard/ppsspp/issues/11813))
* More speed in FF4 effects and other generated curves ([#11425](https://github.com/hrydgard/ppsspp/issues/11425))
* Support for resizing Vulkan on Linux ([#11451](https://github.com/hrydgard/ppsspp/issues/11451))
* Improved support for GLES on Linux/IoT ([#11507](https://github.com/hrydgard/ppsspp/issues/11507), [#11537](https://github.com/hrydgard/ppsspp/issues/11537), [#11541](https://github.com/hrydgard/ppsspp/issues/11541), [#11632](https://github.com/hrydgard/ppsspp/issues/11632), [#11746](https://github.com/hrydgard/ppsspp/issues/11746), [#11806](https://github.com/hrydgard/ppsspp/issues/11806), [#11845](https://github.com/hrydgard/ppsspp/issues/11845))
* Percentage based frameskipping ([#11523](https://github.com/hrydgard/ppsspp/issues/11523))
* DXT accuracy improved, fixing thick white line in Gran Turismo sky ([#11530](https://github.com/hrydgard/ppsspp/issues/11530))
* Fix Motorstorm freeze on non-Windows ([#11618](https://github.com/hrydgard/ppsspp/issues/11618))
* Faster block transfer in some games like Digimon Adventures ([#11531](https://github.com/hrydgard/ppsspp/issues/11531), [#11553](https://github.com/hrydgard/ppsspp/issues/11553), [#11776](https://github.com/hrydgard/ppsspp/issues/11776))
* Blending optimizations and improvements ([#11543](https://github.com/hrydgard/ppsspp/issues/11543), [#11620](https://github.com/hrydgard/ppsspp/issues/11620))
* Improve D3D11 rendering issues ([#11584](https://github.com/hrydgard/ppsspp/issues/11584))
* Change default graphics backend to D3D11 or OpenGL ([#11621](https://github.com/hrydgard/ppsspp/issues/11621), [#11658](https://github.com/hrydgard/ppsspp/issues/11658))
* Remove some outdated settings ([#11665](https://github.com/hrydgard/ppsspp/issues/11665), [#11666](https://github.com/hrydgard/ppsspp/issues/11666), [#11667](https://github.com/hrydgard/ppsspp/issues/11667))
* Fix remote disc streaming with ipv6 ([#11689](https://github.com/hrydgard/ppsspp/issues/11689), [#11700](https://github.com/hrydgard/ppsspp/issues/11700))
* Vulkan: Workarounds for some driver bugs for 5xx series Qualcomm GPUs ([#11691](https://github.com/hrydgard/ppsspp/issues/11691), [#11694](https://github.com/hrydgard/ppsspp/issues/11694))
* Fix some Qt port issues with recent performance improvements ([#11720](https://github.com/hrydgard/ppsspp/issues/11720), [#11807](https://github.com/hrydgard/ppsspp/issues/11807), [#11808](https://github.com/hrydgard/ppsspp/issues/11808))
* UWP Xbox One: fix X/Back button confusion ([#11740](https://github.com/hrydgard/ppsspp/issues/11740))
* Fix Formula 1 2006 timing issue ([#11767](https://github.com/hrydgard/ppsspp/issues/11767))
* Fixes and workarounds for some vertex range culling bugs that broke a few games ([#11785](https://github.com/hrydgard/ppsspp/issues/11785), [#11859](https://github.com/hrydgard/ppsspp/issues/11859)), and disable it on older GPUs ([#11712](https://github.com/hrydgard/ppsspp/issues/11712), [#11773](https://github.com/hrydgard/ppsspp/issues/11773), [#11787](https://github.com/hrydgard/ppsspp/issues/11787))
* Android: Allow putting PSP storage on custom paths like SD cards ([#11812](https://github.com/hrydgard/ppsspp/issues/11812))
* Corrected vocp instruction, fixing models in Artdink games ([#11822](https://github.com/hrydgard/ppsspp/issues/11822), [#11835](https://github.com/hrydgard/ppsspp/issues/11835))
* Bundle SDL in binary for macOS builds ([#11831](https://github.com/hrydgard/ppsspp/issues/11831))
* Prevent keeping games open on Windows ([#11842](https://github.com/hrydgard/ppsspp/issues/11842))

What's new in 1.7.3/1.7.4/1.7.5
-------------------
* Fixes for a couple of common crashes
* Reverted immersive mode change temporarily to see if it helps misaligned buttons
* Change default adhoc server address

What's new in 1.7.2
-------------------
* Update text of "Buy PPSSPP Gold" button

What's new in 1.7.1
-------------------
* Minor crashfixes, lang updates
* Save bug fixed ([#11508](https://github.com/hrydgard/ppsspp/issues/11508))

What's new in 1.7.0
-------------------
* Fix for insta-crash on Galaxy Note 9, some Xiaomi devices, and the new nVidia Shield ([#11441](https://github.com/hrydgard/ppsspp/issues/11441))
* Vertex range culling on most platforms, fixes DTM: Race Driver and similar ([#11393](https://github.com/hrydgard/ppsspp/issues/11393))
* Major speed boost in some Gundam and Earth Defense Force games ([#10973](https://github.com/hrydgard/ppsspp/issues/10973))
* Fix for issues with post processing shaders in GL 3+ ([#11182](https://github.com/hrydgard/ppsspp/issues/11182))
* Fixes to sound output switching on Windows (WASAPI) ([#11438](https://github.com/hrydgard/ppsspp/issues/11438))
* Detects DirectInput devices when running ([#11461](https://github.com/hrydgard/ppsspp/issues/11461))
* Simple Discord integration ([#11307](https://github.com/hrydgard/ppsspp/issues/11307))
* New debugging APIs ([#10909](https://github.com/hrydgard/ppsspp/issues/10909))
* Task switching made a lot more robust (fixes GPD XD problems) ([#11447](https://github.com/hrydgard/ppsspp/issues/11447))
* Texture decoding optimizations ([#11350](https://github.com/hrydgard/ppsspp/issues/11350))
* Tons and tons of miscellaneous bugfixes and compatibility fixes

Looking for [older news](history.md)?


Adhoc support
-------------
Not fully functional, but some games work.  Check the [Ad-Hoc section of the forum](http://forums.ppsspp.org/forumdisplay.php?fid=34) for help.

Credit goes to:
 - Igor Calabria
 - [coldbird's code](https://code.google.com/archive/p/aemu/)
 - Kyhel

