// little-endian "VBSP"   0x50534256
#define IDBSPHEADER	(('P'<<24)+('S'<<16)+('B'<<8)+'V')

// little-endian "LZMA"
#define LZMA_ID	(('A'<<24)|('M'<<16)|('Z'<<8)|('L'))

typedef struct
{
	int	ident;                // BSP file identifier
	int	version;              // BSP file version
	lump_t	lumps[HEADER_LUMPS];  // lump directory array
	int	mapRevision;          // the map's revision (iteration, version) number
} dheader_t;

typedef struct
{
	int	fileofs;	// offset into file (bytes)
	int	filelen;	// length of lump (bytes)
	int	version;	// lump format version
	char	fourCC[4];	// lump ident code
} lump_t;

typedef struct
{
	unsigned int	id;
	unsigned int	actualSize;		// always little endian
	unsigned int	lzmaSize;		// always little endian
	unsigned char	properties[5];
} lzma_header_t;

typedef struct
{
	float x;
	float y;
	float z;
} Vector;

typedef struct
{
	Vector	normal;	// normal vector
	float	dist;	// distance from origin
	int	type;	// plane axis identifier
} dplane_t;

typedef struct
{
	unsigned short	v[2];	// vertex indices
} dedge_t;


typedef struct
{
	unsigned short	planenum;		// the plane number
	byte		side;			// faces opposite to the node's plane direction
	byte		onNode;			// 1 of on node, 0 if in leaf
	int		firstedge;		// index into surfedges
	short		numedges;		// number of surfedges
	short		texinfo;		// texture info
	short		dispinfo;		// displacement info
	short		surfaceFogVolumeID;	// ?
	byte		styles[4];		// switchable lighting info
	int		lightofs;		// offset into lightmap lump
	float		area;			// face area in units^2
	int		LightmapTextureMinsInLuxels[2];	// texture lighting info
	int		LightmapTextureSizeInLuxels[2];	// texture lighting info
	int		origFace;		// original face this was split from
	unsigned short	numPrims;		// primitives
	unsigned short	firstPrimID;
	unsigned int	smoothingGroups;	// lightmap smoothing group
} dface_t;

typedef struct
{
	int	firstside;	// first brushside
	int	numsides;	// number of brushsides
	int	contents;	// contents flags
} dbrush_t;

typedef struct
{
	unsigned short	planenum;	// facing out of the leaf
	short		texinfo;	// texture info
	short		dispinfo;	// displacement info
	short		bevel;		// is the side a bevel plane?
} dbrushside_t;

typedef struct
{
	int		planenum;	// index into plane array
	int		children[2];	// negative numbers are -(leafs + 1), not nodes
	short		mins[3];	// for frustum culling
	short		maxs[3];
	unsigned short	firstface;	// index into face array
	unsigned short	numfaces;	// counting both sides
	short		area;		// If all leaves below this node are in the same area, then
					// this is the area index. If not, this is -1.
	short		paddding;	// pad to 32 bytes length
} dnode_t;

typedef struct
{
	int			contents;		// OR of all brushes (not needed?)
	short			cluster;		// cluster this leaf is in
	short			area:9;			// area this leaf is in
	short			flags:7;		// flags
	short			mins[3];		// for frustum culling
	short			maxs[3];
	unsigned short		firstleafface;		// index into leaffaces
	unsigned short		numleaffaces;
	unsigned short		firstleafbrush;		// index into leafbrushes
	unsigned short		numleafbrushes;
	short			leafWaterDataID;	// -1 for not in water

	//!!! NOTE: for maps of version 19 or lower uncomment this block
	/*
	CompressedLightCube	ambientLighting;	// Precaculated light info for entities.
	short			padding;		// padding to 4-byte boundary
	*/
} dleaf_t;

typedef struct
{
	float	textureVecs[2][4];	// [s/t][xyz offset]
	float	lightmapVecs[2][4];	// [s/t][xyz offset] - length is in units of texels/area
	int	flags;			// miptex flags	overrides
	int	texdata;		// Pointer to texture name, size, etc.
} texinfo_t;

typedef struct
{
	Vector	reflectivity;		// RGB reflectivity
	int	nameStringTableID;	// index into TexdataStringTable
	int	width, height;		// source image
	int	view_width, view_height;
} dtexdata_t;

typedef struct
{
	Vector	mins, maxs;		// bounding box
	Vector	origin;			// for sounds or lights
	int	headnode;		// index into node array
	int	firstface, numfaces;	// index into face array
} dmodel_t;

typedef struct
{
	int	numclusters;
	int	byteofs[numclusters][2]
} dvis_t;

typedef struct
{
	int lumpCount;	// number of game lumps
	dgamelump_t gamelump[lumpCount];
} dgamelumpheader_t;

typedef struct
{
	int		id;		// gamelump ID
	unsigned short	flags;		// flags
	unsigned short	version;	// gamelump version
	int		fileofs;	// offset to this gamelump
	int		filelen;	// length
} dgamelump_t;

typedef struct
{
	int	dictEntries;
	char	name[dictEntries];	// model name
} StaticPropDictLump_t;

typedef struct
{
	int leafEntries;
	unsigned short	leaf[leafEntries];
} StaticPropLeafLump_t;

typedef struct
{
	// v4
	Vector		Origin;		 // origin
	QAngle		Angles;		 // orientation (pitch roll yaw)
        
        // since v11
        float           UniformScale;    // Prop scale

        // v4
	unsigned short	PropType;	 // index into model name dictionary
	unsigned short	FirstLeaf;	 // index into leaf array
	unsigned short	LeafCount;
	unsigned char	Solid;		 // solidity type
	unsigned char	Flags;
	int		Skin;		 // model skin numbers
	float		FadeMinDist;
	float		FadeMaxDist;
	Vector		LightingOrigin;  // for lighting
	// since v5
	float		ForcedFadeScale; // fade distance scale
	// v6 and v7 only
	unsigned short  MinDXLevel;      // minimum DirectX version to be visible
	unsigned short  MaxDXLevel;      // maximum DirectX version to be visible
        // since v8
	unsigned char   MinCPULevel;
	unsigned char   MaxCPULevel;
	unsigned char   MinGPULevel;
	unsigned char   MaxGPULevel;
        // since v7
        color32         DiffuseModulation; // per instance color and alpha modulation
        // since v10
        float           unknown; 
        // since v9
        bool            DisableX360;     // if true, don't show on XBox 360
} StaticPropLump_t;

typedef struct
{
	Vector			startPosition;		// start position used for orientation
	int			DispVertStart;		// Index into LUMP_DISP_VERTS.
	int			DispTriStart;		// Index into LUMP_DISP_TRIS.
	int			power;			// power - indicates size of surface (2^power	1)
	int			minTess;		// minimum tesselation allowed
	float			smoothingAngle;		// lighting smoothing angle
	int			contents;		// surface contents
	unsigned short		MapFace;		// Which map face this displacement comes from.
	int			LightmapAlphaStart;	// Index into ddisplightmapalpha.
	int			LightmapSamplePositionStart;	// Index into LUMP_DISP_LIGHTMAP_SAMPLE_POSITIONS.
	CDispNeighbor		EdgeNeighbors[4];	// Indexed by NEIGHBOREDGE_ defines.
	CDispCornerNeighbors	CornerNeighbors[4];	// Indexed by CORNER_ defines.
	unsigned int		AllowedVerts[10];	// active verticies
} ddispinfo_t;

typedef struct
{
	Vector	vec;	// Vector field defining displacement volume.
	float	dist;	// Displacement distances.
	float	alpha;	// "per vertex" alpha values.
} dDispVert;

typedef struct
{
	unsigned short Tags;	// Displacement triangle tags.
} dDispTri;

typedef struct
{
	int		origin[3];	// position of light snapped to the nearest integer
	int	        size;		// resolution of cubemap, 0 - default
} dcubemapsample_t;

typedef struct
{
	int		Id;
	short		TexInfo;
	unsigned short	FaceCountAndRenderOrder;
	int		Ofaces[OVERLAY_BSP_FACE_COUNT];
	float		U[2];
	float		V[2];
	Vector		UVPoints[4];
	Vector		Origin;
	Vector		BasisNormal;
} doverlay_t;

typedef struct
{
	byte r, g, b;
	signed char exponent;
} ColorRGBExp32;

typedef struct
{
	CompressedLightCube	cube;
	byte x;		// fixed point fraction of leaf bounds
	byte y;		// fixed point fraction of leaf bounds
	byte z;		// fixed point fraction of leaf bounds
	byte pad;	// unused
} dleafambientlighting_t;

typedef struct
{
	ColorRGBExp32 m_Color[6];
} CompressedLightCube;

typedef struct
{
	unsigned short ambientSampleCount;
	unsigned short firstAmbientSample;
} dleafambientindex_t;

typedef struct
{
	int			count;
	doccluderdata_t		data[count];
	int			polyDataCount;
	doccluderpolydata_t	polyData[polyDataCount];
	int			vertexIndexCount;
	int			vertexIndices[vertexIndexCount];
} doccluder_t;

typedef struct
{
	int	flags;
	int	firstpoly;	// index into doccluderpolys
	int	polycount;	// amount of polygons
	Vector	mins;	        // minima of all vertices
	Vector	maxs;	        // maxima of all vertices
	// since v1
	int	area;
} doccluderdata_t;

typedef struct
{
	int	firstvertexindex;	// index into doccludervertindices
	int	vertexcount;		// amount of vertex indices
	int	planenum;
} doccluderpolydata_t;

typedef struct
{
       int modelIndex;  // Perhaps the index of the model to which this physics model applies?
       int dataSize;    // Total size of the collision data sections
       int keydataSize; // Size of the text section
       int solidCount;  // Number of collision data sections
} dphysmodel_t;


