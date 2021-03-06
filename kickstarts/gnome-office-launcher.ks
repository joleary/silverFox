# Maintained by the Fedora Desktop SIG:
# http://fedoraproject.org/wiki/SIGs/Desktop
# mailto:fedora-desktop-list@redhat.com

## NOTE:
# This file should in general be *subtraction* from the gnome-desktop
# comps group to fit within a CD-sized image.  If you want to add
# a package to the desktop, add them to the comps group.

%include fedora-live-base_irl.ks
repo --name=dl_local --baseurl=file:///home/joleary/customRepo
%packages
@games
@graphical-internet
@sound-and-video
@gnome-desktop
@office
-openoffice.org-math
-openoffice.org-math-core
-openoffice.org-draw
-openoffice.org-draw-core
-openoffice.org-calc
-openoffice.org-calc-core
-openoffice.org-pdfimport
-openoffice.org-xsltfilter
-openoffice.org-graphicfilter


nss-mdns
NetworkManager-vpnc
-transmission*

# custom package test
desktopLauncher-0.1

# avoid weird case where we pull in more festival stuff than we need
festival
festvox-slt-arctic-hts

# dictionaries are big
-aspell-*
-hunspell-*
-man-pages*
-words

# save some space
-gnome-user-docs
# no evolution, webmail will suffice
-evolution
-evolution-help
-gnome-games-help
-nss_db
-isdn4k-utils
-dasher
-evince-dvi
-evince-djvu
-constantine-backgrounds
-constantine-backgrounds-extras
-fedora-release*
-fedora-logos
-fedora-icon*
-fedora-gnome-theme
generic-logos
generic-release
-cheese
-abiword

# these pull in excessive dependencies
-ekiga
-tomboy

# things we don't need here
-seahorse
-alacarte
-krb5-auth-dialog
-krb5-workstation
-pam_krb5
-quota
-acpid
-nano
-smartmontools
-minicom
-dos2unix
-finger
-ftp
-jwhois
-mtr
-pinfo
-rsh
-telnet
-unix2dos
-nfs-utils
-ypbind
-yp-tools
-rpcbind

# drop some system-config things
-system-config-boot
-system-config-language
-system-config-lvm
-system-config-network
-system-config-rootpassword
-system-config-services
-policycoreutils-gui

# use the PackageKit helper for bash
PackageKit-command-not-found

# why does the obsoletes not work ?!
-lzma
xz-lzma-compat

# replace bug-buddy by abrt
-abrt-desktop

%end

%post
cat >> /etc/rc.d/init.d/livesys << EOF
# disable screensaver locking
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults -s -t bool /apps/gnome-screensaver/lock_enabled false >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults -s -t bool /desktop/gnome/lockdown/disable_lock_screen true >/dev/null

# set up timed auto-login for after 60 seconds
cat >> /etc/gdm/custom.conf << FOE
[daemon]
#TimedLoginEnable=true
#TimedLogin=liveuser
AutomaticLoginEnable=true
AutomaticLogin=liveuser
#TimedLoginDelay=60
FOE

# Show harddisk install on the desktop
#sed -i -e 's/NoDisplay=true/NoDisplay=false/' /usr/share/applications/liveinst.desktop
mkdir /home/liveuser/Desktop
#cp /usr/share/applications/liveinst.desktop /home/liveuser/Desktop
#chown -R liveuser.liveuser /home/liveuser/Desktop
#chmod a+x /home/liveuser/Desktop/liveinst.desktop

# But not trash and home
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults -s -t bool /apps/nautilus/desktop/trash_icon_visible false >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults -s -t bool /apps/nautilus/desktop/home_icon_visible false >/dev/null

# expand top and bottom panels for session and by default
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/default_setup/toplevels/top_panel/size 48 --type int >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/default_setup/toplevels/bottom_panel/size 48 --type int >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/toplevels/top_panel/size 48 --type int >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/toplevels/bottom_panel/size 48 --type int >/dev/null

# ctrl key to locate the mouse by default
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /desktop/gnome/peripherals/mouse/locate_pointer --type bool true >/dev/null

# show desktop on top panel by dafult
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/default_setup/general/applet_id_list --type list --list-type string [mixer,clock,notification_area,show_desktop_button,window_list] >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/default_setup/general/object_id_list --type list --list-type string [menu_bar] >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/default_setup/applets/show_desktop_button/object_type bonobo-applet --type string >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/default_setup/applets/show_desktop_button/toplevel_id bottom_panel --type string >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/default_setup/applets/show_desktop_button/position 0 --type int >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/default_setup/applets/show_desktop_button/panel_right_stick false --type bool >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/default_setup/applets/show_desktop_button/locked true --type bool >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/default_setup/applets/show_desktop_button/bonobo_iid OAFIID:GNOME_ShowDesktopApplet --type string >/dev/null


gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/general/applet_id_list --type list --list-type string [mixer,clock,notification_area,show_desktop_button,window_list] >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/general/object_id_list --type list --list-type string [menu_bar] >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/applets/show_desktop_button/object_type bonobo-applet --type string >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/applets/show_desktop_button/toplevel_id bottom_panel --type string >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/applets/show_desktop_button/position 0 --type int >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/applets/show_desktop_button/panel_right_stick false --type bool >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/applets/show_desktop_button/locked true --type bool >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/panel/applets/show_desktop_button/bonobo_iid OAFIID:GNOME_ShowDesktopApplet --type string >/dev/null

# remove nautilus as desktop owner
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/nautilus/preferences/show_desktop false --type bool >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/nautilus/preferences/always_use_browser true --type bool >/dev/null
gconftool-2 --direct --config-source=xml:readwrite:/etc/gconf/gconf.xml.defaults --set /apps/nautilus/preferences/always_use_location_entry true --type bool >/dev/null


# Turn off PackageKit-command-not-found while uninstalled
sed -i -e 's/^SoftwareSourceSearch=true/SoftwareSourceSearch=false/' /etc/PackageKit/CommandNotFound.conf

EOF

%end
