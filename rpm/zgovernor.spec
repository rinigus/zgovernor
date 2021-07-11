Name: zgovernor
Version: 0.2
Release: 1

Summary: CPU and GPU Governor
License: GPLv3+
URL:     https://github.com/rinigus/zgovernor

Source:  %{name}-%{version}.tar.xz

BuildRequires: cmake
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(keepalive)

%description
Set CPU, GPU and other properties depending on screen state

%prep
%setup -q

%build

mkdir build-rpm || true
cd build-rpm

cmake \
    -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_VERBOSE_MAKEFILE=ON \
    -DVERSION='%{version}-%{release}' \
    ..

make %{?_smp_mflags}

%install
rm -rf %{buildroot}

#mkdir -p %{buildroot}%{_sysconfdir}
#cp example/zgovernor.ini %{buildroot}%{_sysconfdir}/zgovernor.ini

mkdir -p %{buildroot}%{_unitdir}
cp zgovernor.service %{buildroot}%{_unitdir}

cd build-rpm
make DESTDIR=%{buildroot} install

%preun
if [ "$1" -eq 0 ]; then
    systemctl disable %{name}.service || :
    systemctl stop %{name}.service || :
fi

%post
systemctl daemon-reload || :
systemctl reload-or-try-restart %{name}.service || :
systemctl enable %{name}.service || :
systemctl start %{name}.service || :

%postun
systemctl daemon-reload || :


%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
#%config(noreplace) %{_sysconfdir}/zgovernor.ini
%{_unitdir}/%{name}.service
