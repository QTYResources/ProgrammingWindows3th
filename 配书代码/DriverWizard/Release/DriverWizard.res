        ��  ��                  �      �� ��     0         �4   V S _ V E R S I O N _ I N F O     ���               ?                         N   S t r i n g F i l e I n f o   *   0 4 0 9 0 4 B 0        C o m p a n y N a m e     V   F i l e D e s c r i p t i o n     D R I V E R W I Z A R D   A p p W i z a r d     6   F i l e V e r s i o n     1 ,   0 ,   0 ,   1     :   I n t e r n a l N a m e   D R I V E R W I Z A R D     :   L e g a l C o p y r i g h t   C o p y r i g h t       (    L e g a l T r a d e m a r k s     J   O r i g i n a l F i l e n a m e   D R I V E R W I Z A R D . D L L     <   P r o d u c t N a m e     D r i v e r   W i z a r d   :   P r o d u c t V e r s i o n   1 ,   0 ,   0 ,   1     D    V a r F i l e I n f o     $    T r a n s l a t i o n     	�  D   T E M P L A T E   N E W P R O J . I N F         0        $$// newproj.inf = template for list of template files
$$//  format is 'sourceResName' \t 'destFileName'
$$//    The source res name may be preceded by any combination of '=', '-',  '!', '?', ':', '#', and/or '*'
$$//       '=' => the resource is binary
$$//       '-' => the file should not be added to the project (all files are added to the project by default)
$$//       '!' => the file should be marked exclude from build
$$//       '?' => the file should be treated as a help file
$$//       ':' => the file should be treated as a resource
$$//       '#' => the file should be treated as a template (implies '!')
$$//       '*' => bypass the custom AppWizard's resources when loading
$$//	if name starts with / => create new subdir





ROOT.CPP	$$root$$.cpp


  !   D   T E M P L A T E   C O N F I R M . I N F         0        ��Ҫ�������ļ���
	$$Root$$.cpp
   M  <   T E M P L A T E   R O O T . C P P       0        //////////////////////////////////////////////////
// $$root$$.cpp�ļ�


extern "C"
{
	#include <ntddk.h>
}


// �����������ʱ����DriverEntry����
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{

	// �����������DriverEntry����ִ�н��
	return STATUS_DEVICE_CONFIGURATION_ERROR;
}
   �      �� ��     0        (       @                                  �  �   �� �   � � ��  ��� ���   �  �   �� �   � � ��  ���       N� �          N� �    ������N��������wwwwwG�q��www��������D�������������p��������������p��������������p����������� � p� ��������� �x� ���������w�wwx������������www��������������w���������������w���������������w��������������������������������������������������������������������������������������������������������������������������������������wwwwwwwwwwwwww��DDDDDDDDD@    ��DDDDDDDDDGpwp��DDDDDDDDDGpwp��DDDDDDDDDDDDDD��wwwwwwwwwwwwww�����������������                                ��`�                                                                                                                    ��������       �� ��     0                 �   