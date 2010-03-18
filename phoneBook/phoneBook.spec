Name:           phoneBook
Version:        0.1
Release:        1%{?dist}
Summary:        A simple book book application.

Group:          User Interface/Accessories
License:        To Be Determined
URL:            None
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  gtk2-devel, desktop-file-utils
Requires:       gtk2



%description
A simple phone book application that displays some usefull numbers.

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
/usr/share/applications/%{name}.desktop
/usr/bin/%{name}
/usr/share/%{name}-%{version}/%{name}




%changelog
