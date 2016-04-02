Summary: Set uid/pid/supplementary groups
Name: setusergroups
Version: 0.1.0
Release: 1%{?dist}
License: MIT
Group: Applications/Utilities
URL: https://github.com/tokuhirom/setusergroups
Source: https://github.com/tokuhirom/setusergroups/archive/master.zip
BuildRoot:      %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
BuildRequires: make
BuildRequires: autoconf >= 2.68
BuildRequires: automake

%description
setusergroups - set uid/gid/supplementary groups

%prep
%setup -q -n setusergroups-master

%build
%{__rm} -rf %{buildroot}
autoreconf -i
./configure --bindir=%{_bindir}

%install
%{__make}
%{__make} install DESTDIR="%{buildroot}"

%files
%{_bindir}/%{name}
%doc %{_prefix}/local/share/man/man1/setusergroups.1

%clean
%{__rm} -rf %{buildroot}

%changelog
* Tue Feb 9 2016 KUCHIKI Taku <kuchiki.taku@gmail.com> - 0.1.0-1
- Initial package
