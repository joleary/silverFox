Name:           desktopLauncher
Version:        0.1
Release:        1%{?dist}
Summary:        Desktop application launcher.

Group:          User Interface/Desktops
License:        To Be Determined
URL:            None
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  gtk2-devel, desktop-file-utils
Requires:       gtk2



%description
A desktop application launcher that presents applications in a configurable,
easy manner.

%prep
%setup -q


%build
make all

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
/etc/xdg/autostart/%{name}.desktop
/usr/share/applications/%{name}.desktop
/usr/bin/gmail
/usr/bin/facebook
/usr/bin/firefox-homeLauncher
/usr/share/%{name}-%{version}/%{name}
/usr/share/%{name}-%{version}/config.xml
/usr/share/%{name}-%{version}/resources/facebook.png
/usr/share/%{name}-%{version}/resources/firefox.png
/usr/share/%{name}-%{version}/resources/gmail.png
/usr/share/%{name}-%{version}/resources/gnome-calculator.png
/usr/share/%{name}-%{version}/resources/openoffice2.png
/usr/share/%{name}-%{version}/resources/images/icon_facebook.png
/usr/share/%{name}-%{version}/resources/images/gmail.png
/usr/share/%{name}-%{version}/resources/styles/base_style.css
/usr/share/%{name}-%{version}/resources/homepage.html




%changelog
