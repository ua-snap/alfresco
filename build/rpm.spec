Name:		alfresco
Version:	%{version}
Release:	%{release}
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

%define inst_dir /usr/bin

%description
This package provides the ALFRESCO modeling tool, in command line form.

%prep
%setup -c


%build
%configure
./configure CXX=mpicxx
make %{?_smp_mflags}


%install
rm -rf ${RPM_BUILD_ROOT}

mkdir -p ${RPM_BUILD_ROOT}/%{inst_dir}/

cp fresco ${RPM_BUILD_ROOT}/%{inst_dir}
cp frescocli ${RPM_BUILD_ROOT}/%{inst_dir}

%clean
rm -rf ${RPM_BUILD_ROOT}


%files
%defattr(-,root,root,-)
%doc
%{inst_dir}/fresco
%{inst_dir}/frescocli



%changelog

