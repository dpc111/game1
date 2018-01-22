BUILD = ../proto_pb
	
PROTOSRCS = common.proto \
			hall.proto \
 			battle.proto \

PROTO :=

define PROTO_temp
  	TAR :=  $(BUILD)/$(notdir $(basename $(1)))
  	PROTO := $(PROTO) $$(TAR).pb
  	$$(TAR).pb : | $(BUILD)
  	$$(TAR).pb : $(1)
		protoc -o$$@ $$<
endef

$(foreach s,$(PROTOSRCS),$(eval $(call PROTO_temp,$(s))))

all : $(PROTO) proto

proto:
	../../3rd/protobuf/bin/protoc --cpp_out=../proto_cpp *.proto
	#protoc --cpp_out=../proto_cpp *.proto

clean:
	rm ../proto_pb/*
	rm ../proto_cpp/*
