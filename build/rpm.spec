Name:		alfresco
Version:	%{version}
Release:	%{release}.el6
Summary:	ALFRESCO Modeling Tool

Group:		Productivity/Scientific/Other 
License:	TBD
URL:		http://snap.uaf.edu
Source0:	alfresco.tgz
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:	openmpi-devel, gdal-devel, poco-foundation, poco-xml, poco-net, boost-devel
Requires:	openmpi
Requires:	gdal
Requires:	poco-foundation, poco-xml, poco-net
Requires:	boost

%define inst_dir /usr

%description
This package provides the ALFRESCO modeling tool, in command line form.

%prep
%setup -c


%build
%configure
./configure --prefix=${RPM_BUILD_ROOT}/%{inst_dir}
make %{?_smp_mflags}
make install


%install
rm -rf ${RPM_BUILD_ROOT}

mkdir -p ${RPM_BUILD_ROOT}/%{inst_dir}/bin
mkdir -p ${RPM_BUILD_ROOT}/%{inst_dir}/lib64
mkdir -p ${RPM_BUILD_ROOT}/%{inst_dir}/include

cp src/fresco ${RPM_BUILD_ROOT}/%{inst_dir}/bin
cp mpi/fresco-mpi ${RPM_BUILD_ROOT}/%{inst_dir}/bin
cp lib/.libs/*.so* ${RPM_BUILD_ROOT}/%{inst_dir}/lib64

%clean
rm -rf ${RPM_BUILD_ROOT}


%files
%defattr(-,root,root,-)
%doc
%{inst_dir}/bin/fresco
%{inst_dir}/bin/fresco-mpi
%{inst_dir}/lib64/*.so*



%changelog

