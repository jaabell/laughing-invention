%old_element should have 1 index, 8 node numbers, and 1 material. total:10.
%old_node should have 3 coordinates. No index.
load 'element_node.dat';
load 'node_coordinate.dat';
if size(element_node,2)>13
    element=[element_node(:,1) element_node(:,7:14) ones(size(element_node,1),1)];
else
    element=element_node;
end
node = node_coordinate(:,2:4);
clear element_node node_coordinate;
n=size(element,1);
nodenumber=length(node);
%element: 1 is index. 2-9 is the origin nodes in 8NBrick. 10 is material
Newelement=[element(:,1:9) zeros(n,19) element(:,10)];   
Newnode=[node;zeros(length(node)*3,3)];
nodecord=zeros(27,3);
clear element node;
for i=1:n
    nodecord(9,:)=0.5*(Newnode(Newelement(i,2),:)+Newnode(Newelement(i,3),:));
    nodecord(10,:)=0.5*(Newnode(Newelement(i,3),:)+Newnode(Newelement(i,4),:));
    nodecord(11,:)=0.5*(Newnode(Newelement(i,4),:)+Newnode(Newelement(i,5),:));
    nodecord(12,:)=0.5*(Newnode(Newelement(i,2),:)+Newnode(Newelement(i,5),:));
    nodecord(13,:)=0.5*(Newnode(Newelement(i,6),:)+Newnode(Newelement(i,7),:));
    nodecord(14,:)=0.5*(Newnode(Newelement(i,7),:)+Newnode(Newelement(i,8),:));
    nodecord(15,:)=0.5*(Newnode(Newelement(i,8),:)+Newnode(Newelement(i,9),:));
    nodecord(16,:)=0.5*(Newnode(Newelement(i,6),:)+Newnode(Newelement(i,9),:));
    nodecord(17,:)=0.5*(Newnode(Newelement(i,2),:)+Newnode(Newelement(i,6),:));
    nodecord(18,:)=0.5*(Newnode(Newelement(i,3),:)+Newnode(Newelement(i,7),:));
    nodecord(19,:)=0.5*(Newnode(Newelement(i,4),:)+Newnode(Newelement(i,8),:));
    nodecord(20,:)=0.5*(Newnode(Newelement(i,5),:)+Newnode(Newelement(i,9),:));
    nodecord(22,:)=0.5*(nodecord(9,:)+nodecord(13,:));
    nodecord(23,:)=0.5*(nodecord(10,:)+nodecord(14,:));
    nodecord(24,:)=0.5*(nodecord(11,:)+nodecord(15,:));
    nodecord(25,:)=0.5*(nodecord(12,:)+nodecord(16,:));
    nodecord(21,:)=0.5*(nodecord(22,:)+nodecord(24,:));
    nodecord(26,:)=0.5*(nodecord(9,:)+nodecord(11,:));
    nodecord(27,:)=0.5*(nodecord(13,:)+nodecord(15,:));
    for j=9:27
        addnewnode=0;
        nodeexist=find((Newnode(:,1)==nodecord(j,1)).*(Newnode(:,2)==nodecord(j,2)).*(Newnode(:,3)==nodecord(j,3))==1);
        nexist=length(nodeexist);
        if nexist>1
            nexist
            i
        end
        for k=1:nexist
            [p,~]=find(Newelement(1:i-1,2:28)==nodeexist(k));
            if length(intersect(Newelement(p,2:9),Newelement(i,2:9)))>=2
                Newelement(i,j+1)=nodeexist(k);
                addnewnode=1;
                break;
            end
        end
        if addnewnode==0
            nodenumber=nodenumber+1;
            Newnode(nodenumber,:)=nodecord(j,:);
            Newelement(i,j+1)=nodenumber;
        end
        %         if isempty(nodeexist)
        %             nodenumber=nodenumber+1;
        %             Newnode(nodenumber,:)=nodecord(j,:);
        %             Newelement(i,j+1)=nodenumber;
        %         else
        %             Newelement(i,j+1)=nodeexist;
        %         end
    end
end
Newnode=Newnode(1:nodenumber,:);



% newnode only have 3 coordinates.
%Newelement have 1 index,27nodes, 1 materials..total:29..
index=[1:1:size(Newnode,1)]';
writenode=[index Newnode];

fileID = fopen('node.fei','w');
formatSpec='add node # %8.0d at ( %8.4f *m, %8.4f *m,%8.4f *m) with 3 dofs;\n';
fprintf(fileID,formatSpec,writenode');    %matlab is column-first.
fclose(fileID);

fileID2 = fopen('element.fei','w');
formatSpec='add element #  %8.0d type 27NodeBrick with nodes(%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d) use material # %8.0d; \n';
fprintf(fileID2,formatSpec,Newelement');
fclose(fileID2);

fileID3 = fopen('elementLT.fei','w');
formatSpec='add element #  %8.0d type 27NodeBrickLT with nodes(%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d,%8.0d) use material # %8.0d; \n';
fprintf(fileID3,formatSpec,Newelement');
fclose(fileID3);

% fileID = fopen('contact_x.fei','w');
% formatSpec='add element # %8.0d type contact with nodes ( %8.0d , %8.0d   ) normal_stiffness = 1.0e12*N/m tangential_stiffness = 1.0e12*N/m friction_ratio = 0.4 contact_plane_vector = (1.0,0.0,0.0); \n';
% fprintf(fileID,formatSpec,contactx);
% fclose(fileID);
% 
% fileID = fopen('contact_y.fei','w');
% formatSpec='add element # %8.0d type contact with nodes ( %8.0d , %8.0d   ) normal_stiffness = 1.0e12*N/m tangential_stiffness = 1.0e12*N/m friction_ratio = 0.4 contact_plane_vector = (0.0,1.0,0.0); \n';
% fprintf(fileID,formatSpec,contacty);
% fclose(fileID);
% 
% fileID = fopen('contact_z.fei','w');
% formatSpec='add element # %8.0d type contact with nodes ( %8.0d , %8.0d   ) normal_stiffness = 1.0e12*N/m tangential_stiffness = 1.0e12*N/m friction_ratio = 0.4 contact_plane_vector = (0.0,0.0,1.0); \n';
% fprintf(fileID,formatSpec,contactz);
% fclose(fileID);

